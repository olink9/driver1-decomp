# Driver 1 PC port on Windows: set up from your own disc, then play.
#
#   driver1 setup <disc.cue|disc.bin|disc.chd>   once: unpack your disc, build the game
#   driver1                                       play (fullscreen)
#   driver1 --window                              play in a window
#
# (driver1.cmd runs this script: from a command prompt as above, or drop your .cue/.bin/.chd onto driver1.cmd.)
# Optional: everything this does is also described step by step in port/README.md.
# Needs: Windows 10/11 (64-bit). Setup offers to install what is missing and asks first: MSYS2 (the compiler,
# 32-bit MinGW-w64) and Python 3. Nothing from the game is downloaded or included: it all comes from your disc.
$ErrorActionPreference = 'Continue'
$Root = $PSScriptRoot
Set-Location -LiteralPath $Root
[Environment]::CurrentDirectory = $Root   # (what the native tools start in: Set-Location doesn't change it)
$SlusSha1 = '94b44a84de2f9fa2dcaf120db2e7b3d74bc70d3d'
# OpenAL Soft (the sound library, LGPL): MSYS2 has no 32-bit package, so setup builds it from its source release
$OpenAlVersion = '1.25.2'
$OpenAlUrl = "https://github.com/kcat/openal-soft/archive/refs/tags/$OpenAlVersion.tar.gz"
$OpenAlSha256 = 'fb27e5839aa11f0e5b9d33756965291fad5d6909ab928ea1f796f4a1a6877894'
$MsysPackages = @('mingw-w64-i686-gcc', 'mingw-w64-i686-binutils', 'mingw-w64-i686-cmake', 'mingw-w64-i686-ninja',
                  'mingw-w64-i686-pkgconf', 'mingw-w64-i686-SDL2')
$script:Yes = $false

function Die([string]$msg) { Write-Host ''; Write-Host "Error: $msg" -ForegroundColor Red; exit 1 }
function Say([string]$msg) { Write-Host $msg }
function Sha1([string]$p) { (Get-FileHash -Algorithm SHA1 -LiteralPath $p).Hash.ToLower() }
function Ask([string]$q) {
    if ($script:Yes) { return $true }
    $a = Read-Host "$q [y/N]"
    return $a -match '^\s*(y|yes|j|ja)\s*$'
}
function Usage {
    Get-Content -LiteralPath $PSCommandPath -TotalCount 10 | Select-Object -Skip 1 | ForEach-Object { $_ -replace '^# ?', '' }
    exit 0
}
# a native command with its output (stdout and stderr) in a log file; returns the exit code
function RunLog([string]$log, [string]$cmdline) {
    cmd /s /c "`"($cmdline) > `"$log`" 2>&1`""
    return $LASTEXITCODE
}

function Find-Msys {
    foreach ($c in @($env:MSYS2_ROOT, 'C:\msys64')) {
        if ($c -and (Test-Path -LiteralPath "$c\usr\bin\pacman.exe")) { return $c }
    }
    $keys = 'HKCU:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*',
            'HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*'
    foreach ($k in (Get-ItemProperty $keys -ErrorAction SilentlyContinue)) {
        if ($k.DisplayName -like 'MSYS2*' -and $k.InstallLocation -and
            (Test-Path -LiteralPath "$($k.InstallLocation)\usr\bin\pacman.exe")) { return $k.InstallLocation }
    }
    return $null
}

# a command in the MSYS2 shell, its output on the console; $LASTEXITCODE is its exit code
function Msys([string]$msys, [string]$cmd) {
    $env:MSYSTEM = 'MINGW32'
    & "$msys\usr\bin\bash.exe" -lc $cmd | Out-Host
}

# Python 3.9+ (python.org or winget; not MSYS2's: splat's helper packages ship Windows wheels for it)
function Find-Python {
    if (Get-Command py -ErrorAction SilentlyContinue) {
        foreach ($v in '-3.13', '-3.12', '-3.11', '-3.10', '-3') {
            $out = & py $v -c "import sys; print(sys.executable if sys.version_info >= (3, 9) else '')" 2>$null
            if ($LASTEXITCODE -eq 0 -and $out) { return "$out".Trim() }
        }
    }
    foreach ($c in (Get-Command python -All -ErrorAction SilentlyContinue)) {
        if ($c.Source -like '*\WindowsApps\*') { continue }   # the Microsoft Store placeholder
        $out = & $c.Source -c "import sys; print(sys.executable if sys.version_info >= (3, 9) else '')" 2>$null
        if ($LASTEXITCODE -eq 0 -and $out) { return "$out".Trim() }
    }
    return $null
}

function Install-Tools {
    $msys = Find-Msys
    $py = Find-Python
    $missing = @()
    if ($msys) {
        & "$msys\usr\bin\bash.exe" -lc true | Out-Null    # (a new MSYS2's first start sets itself up and says so)
        $missing = @(& "$msys\usr\bin\bash.exe" -lc "pacman -T $($MsysPackages -join ' ')" 2>$null | Where-Object { $_ })
    }
    if ($msys -and $py -and $missing.Count -eq 0) { return @($msys, $py) }

    $winget = Get-Command winget -ErrorAction SilentlyContinue
    Say ''
    Say 'Setup needs these and will install them:'
    if (-not $msys) { Say '  - MSYS2 (compiler environment, ~1 GB in C:\msys64): winget install MSYS2.MSYS2' }
    if (-not $msys -or $missing.Count) {
        $list = if ($msys) { $missing } else { $MsysPackages }
        Say "  - in MSYS2, the 32-bit MinGW-w64 toolchain and SDL2: pacman -S $($list -join ' ')"
        Say '    (after bringing MSYS2 up to date first: pacman -Syu)'
    }
    if (-not $py) { Say '  - Python 3.13 (python.org build): winget install Python.Python.3.13' }
    if (-not (Ask 'Install them now?')) {
        Die 'nothing installed. Install the above yourself (see port/README.md, "Windows"), then run setup again'
    }
    if (-not $msys) {
        if (-not $winget) { Die 'winget is missing: install MSYS2 from https://www.msys2.org (to C:\msys64), then run setup again' }
        winget install --id MSYS2.MSYS2 -e --accept-source-agreements --accept-package-agreements
        $msys = Find-Msys
        if (-not $msys) { Die 'MSYS2 did not install (see above). Install it from https://www.msys2.org, then run setup again' }
        & "$msys\usr\bin\bash.exe" -lc true | Out-Null
    }
    # MSYS2 installs from an up-to-date package list only (twice: the second run finishes what the first had to
    # stop for when MSYS2's own core was updated)
    Msys $msys 'pacman -Syu --noconfirm'
    Msys $msys 'pacman -Syu --noconfirm'
    Msys $msys "pacman -S --needed --noconfirm $($MsysPackages -join ' ')"
    if ($LASTEXITCODE -ne 0) { Die 'pacman could not install the packages (see above)' }
    if (-not $py) {
        if (-not $winget) { Die 'winget is missing: install Python 3 from https://www.python.org, then run setup again' }
        winget install --id Python.Python.3.13 -e --accept-source-agreements --accept-package-agreements
        $env:PATH = [Environment]::GetEnvironmentVariable('PATH', 'User') + ';' + $env:PATH
        $py = Find-Python
        if (-not $py) { Die 'Python did not install (see above). Install it from https://www.python.org, then run setup again' }
    }
    return @($msys, $py)
}

# OpenAL Soft, built once into .deps\openal with the MSYS2 toolchain (or D1_OPENAL=<dir>: a 32-bit OpenAL you
# already have there, as bin\OpenAL32.dll + lib\pkgconfig\openal.pc + include\AL)
function Build-OpenAl([string]$msys) {
    if ($env:D1_OPENAL) {
        if (-not (Test-Path -LiteralPath "$env:D1_OPENAL\bin\OpenAL32.dll")) { Die "D1_OPENAL=${env:D1_OPENAL}: no bin\OpenAL32.dll there" }
        return $env:D1_OPENAL
    }
    $oal = "$Root\.deps\openal"
    if (Test-Path -LiteralPath "$oal\bin\OpenAL32.dll") { return $oal }
    New-Item -ItemType Directory -Force "$Root\.deps" | Out-Null
    $tgz = "$Root\.deps\openal-soft-$OpenAlVersion.tar.gz"
    Say "Building OpenAL Soft $OpenAlVersion (the sound library, open source; once) ..."
    if (-not (Test-Path -LiteralPath $tgz) -or (Get-FileHash -LiteralPath $tgz -Algorithm SHA256).Hash.ToLower() -ne $OpenAlSha256) {
        [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
        try { Invoke-WebRequest -UseBasicParsing -Uri $OpenAlUrl -OutFile $tgz } catch { Die "could not download $OpenAlUrl ($_)" }
        if ((Get-FileHash -LiteralPath $tgz -Algorithm SHA256).Hash.ToLower() -ne $OpenAlSha256) {
            Remove-Item -LiteralPath $tgz
            Die "the OpenAL Soft download is not the expected file (sha256 $OpenAlSha256)"
        }
    }
    $src = "$Root\.deps\openal-soft-$OpenAlVersion"
    if (Test-Path -LiteralPath $src) { Remove-Item -Recurse -Force -LiteralPath $src }
    & "$env:SystemRoot\System32\tar.exe" -xzf $tgz -C "$Root\.deps"
    if ($LASTEXITCODE -ne 0) { Die "could not unpack $tgz" }
    $bld = "$Root\.deps\openal-build"
    $log = "$Root\.deps\openal-build.log"
    $rc = RunLog $log "cmake -S `"$src`" -B `"$bld`" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`"$oal`" -DALSOFT_UTILS=OFF -DALSOFT_EXAMPLES=OFF -DALSOFT_TESTS=OFF && ninja -C `"$bld`" install"
    if ($rc -ne 0 -or -not (Test-Path -LiteralPath "$oal\bin\OpenAL32.dll")) { Die "building OpenAL Soft failed, see $log" }
    return $oal
}

# the DLLs a program needs from the toolchain and OpenAL, recursively (Windows' own ones stay Windows')
function Copy-Dlls([string]$exe, [string[]]$dirs, [string]$dest) {
    $todo = New-Object System.Collections.Queue
    $todo.Enqueue($exe)
    $seen = @{}
    while ($todo.Count) {
        $f = $todo.Dequeue()
        foreach ($l in (objdump -p $f 2>$null)) {
            if ($l -notmatch 'DLL Name: (.+)$') { continue }
            $name = $Matches[1].Trim()
            if ($seen.ContainsKey($name.ToLower())) { continue }
            $seen[$name.ToLower()] = 1
            foreach ($d in $dirs) {
                if (Test-Path -LiteralPath "$d\$name") {
                    Copy-Item -LiteralPath "$d\$name" -Destination $dest -Force
                    $todo.Enqueue("$d\$name")
                    break
                }
            }
        }
    }
}

function Setup([string[]]$a) {
    $disc = $null
    foreach ($x in $a) {
        switch -regex ($x) {
            '^(-h|--help)$' { Usage }
            '^(-y|--yes)$' { $script:Yes = $true; break }
            '^-' { Die "unknown option $x" }
            default { $disc = $x }
        }
    }
    if (-not $disc) { Die 'which disc? driver1 setup <path to your .cue, .bin or .chd>' }
    if (-not (Test-Path -LiteralPath $disc -PathType Leaf)) { Die "${disc}: no such file" }
    $disc = (Resolve-Path -LiteralPath $disc).Path
    switch -regex ($disc.ToLower()) {
        '\.(cue|bin|chd)$' { break }
        '\.pbp$' { Die 'a .pbp (PSP eboot) has to be converted to bin/cue first, e.g. with psxtract or pop-fe' }
        '\.iso$' { Die "a .iso only holds 2048-byte sectors, but the game's music and videos need the raw 2352-byte ones. Rip the disc as bin/cue (or convert to .chd)" }
        default { Die "${disc}: expected a .cue, .bin or .chd" }
    }

    # 0. The tools (asks before installing anything)
    $tools = Install-Tools
    $msys = $tools[0]; $py = $tools[1]
    $env:PATH = "$msys\mingw32\bin;$env:PATH"

    # 1. The disc image: a raw bin/cue as is; a CHD converted with chdman first.
    switch -regex ($disc.ToLower()) {
        '\.chd$' {
            if (-not (Get-Command chdman -ErrorAction SilentlyContinue)) {
                Die 'a .chd needs chdman to convert it (it comes with MAME: https://www.mamedev.org - put chdman.exe on your PATH)'
            }
            Say "Converting $disc to bin/cue ..."
            New-Item -ItemType Directory -Force "$Root\.disc" | Out-Null
            Remove-Item -Force -ErrorAction SilentlyContinue "$Root\.disc\driver1.cue", "$Root\.disc\driver1.bin"
            if ((RunLog "$Root\.disc\chdman.log" "chdman extractcd -i `"$disc`" -o `"$Root\.disc\driver1.cue`" -ob `"$Root\.disc\driver1.bin`"") -ne 0) {
                Die 'chdman could not convert it, see .disc\chdman.log'
            }
            $disc = "$Root\.disc\driver1.cue"
        }
    }

    # 2. Unpack it and check it is the version this port is built for.
    Say 'Unpacking the disc into pcdata\ ...'
    if (Test-Path pcdata.new) { Remove-Item -Recurse -Force pcdata.new }
    & $py port\pc\pc_extract.py $disc pcdata.new | Out-Null
    if ($LASTEXITCODE -ne 0) { Die 'unpacking failed (see the message above)' }
    if (-not (Test-Path pcdata.new\SLUS_008.42)) {
        Remove-Item -Recurse -Force pcdata.new
        Die 'this is not Driver (US): no SLUS_008.42 on the disc'
    }
    if ((Sha1 pcdata.new\SLUS_008.42) -ne $SlusSha1) {
        Remove-Item -Recurse -Force pcdata.new
        Die "this is a different version of Driver. The port needs the US release, Rev 1 (SLUS-008.42), whose SLUS_008.42 has sha1 $SlusSha1"
    }
    if (Test-Path pcdata) { Remove-Item -Recurse -Force pcdata }
    Rename-Item pcdata.new pcdata

    # 3. The original executable and code overlays, for the build to split the game's data out of.
    New-Item -ItemType Directory -Force orig, build | Out-Null
    Copy-Item pcdata\SLUS_008.42 orig\SLUS_008.42
    foreach ($f in 'FRONTEND', 'MCARD', 'CRED', 'IVIEW') {
        Copy-Item "pcdata\DRIVER\$f.BIN" "orig\$f.BIN"
        $want = ((Get-Content "$($f.ToLower()).sha1" -TotalCount 1) -split '\s+')[0]
        if ((Sha1 "orig\$f.BIN") -ne $want) { Die "orig\$f.BIN does not match $($f.ToLower()).sha1" }
    }

    # 4. Split the game's data out (splat, at the versions the Linux build container pins).
    if (-not (Test-Path .venv-pc\Scripts\python.exe)) {
        Say 'Preparing the Python tools (once) ...'
        & $py -m venv .venv-pc
        $pins = [regex]::Matches((Get-Content -Raw port\container\Containerfile), '[A-Za-z0-9_-]+==[0-9.]+') | ForEach-Object { $_.Value }
        if ((RunLog "$Root\build\pip.log" ".venv-pc\Scripts\python.exe -m pip install $($pins -join ' ')") -ne 0) {
            Remove-Item -Recurse -Force .venv-pc
            Die 'installing the Python tools failed, see build\pip.log'
        }
    }
    if (-not (Test-Path build\main_syms.txt)) { Copy-Item port\container\main_syms.txt build\main_syms.txt }
    foreach ($y in 'slus_008.42', 'frontend', 'mcard', 'cred', 'iview') {
        Say "  splitting $y"
        if ((RunLog "$Root\build\split-$y.log" ".venv-pc\Scripts\python.exe -m splat split $y.yaml") -ne 0) {
            Die "splat failed on $y.yaml, see build\split-$y.log"
        }
    }

    # 5. Compile the PC program (32-bit MinGW-w64, SDL2 + OpenAL, PsyCross).
    $oal = Build-OpenAl $msys
    $env:PKG_CONFIG_PATH = "$oal\lib\pkgconfig"
    Say '  compiling (this takes a few minutes)'
    $vpy = "$Root\.venv-pc\Scripts\python.exe"
    if ((RunLog "$Root\build\cmake.log" "cmake -S port -B build-pc -G Ninja -DPython3_EXECUTABLE=`"$vpy`"") -ne 0) {
        Die 'cmake failed, see build\cmake.log'
    }
    if ((RunLog "$Root\build\make.log" 'ninja -C build-pc') -ne 0) { Die 'compiling failed, see build\make.log' }

    # 6. The program next to the open-source DLLs it needs (SDL2, OpenAL, the MinGW runtime).
    if (Test-Path play) { Get-ChildItem play -Filter *.dll | Remove-Item -Force }
    New-Item -ItemType Directory -Force play | Out-Null
    Copy-Item build-pc\driver1.exe, build-pc\driver1.sym play\ -Force
    Copy-Dlls "$Root\play\driver1.exe" @("$msys\mingw32\bin", "$oal\bin") "$Root\play"
    Say "  built: play\driver1.exe, with $(@(Get-ChildItem play -Filter *.dll).Count) libraries next to it"
    Say ''
    Say 'Done. Play with:  driver1    (or driver1 --window)'
}

function Play([string[]]$a) {
    $fullscreen = '1'
    foreach ($x in $a) {
        switch -regex ($x) {
            '^--window$' { $fullscreen = '0'; break }
            '^(-h|--help)$' { Usage }
            default { Die "unknown option $x (setup goes first: driver1 setup <disc>)" }
        }
    }
    if (-not (Test-Path play\driver1.exe) -or -not (Test-Path pcdata)) {
        Die 'not set up yet: driver1 setup <path to your disc image> (or drop the .cue onto driver1.cmd)'
    }
    # Saves (card1.mcd), settings (driver1_mods.ini) and the log live in play\.
    $env:D1_DATA = "$Root\pcdata"
    if (-not $env:D1_FULLSCREEN) { $env:D1_FULLSCREEN = $fullscreen }
    Say 'Driver is running. The log is play\driver1.log.'
    cmd /s /c "`"cd /d `"$Root\play`" && .\driver1.exe >> driver1.log 2>&1`""
}

$cmd = if ($args.Count) { "$($args[0])" } else { '' }
switch -regex ($cmd) {
    '^setup$' { Setup @($args | Select-Object -Skip 1) }
    '^(-h|--help|help)$' { Usage }
    default {
        # a disc image dropped onto driver1.cmd: set up from it
        if ($args.Count -eq 1 -and $cmd -match '\.(cue|bin|chd|iso|pbp)$' -and (Test-Path -LiteralPath $cmd)) { Setup @($cmd) }
        else { Play @($args) }
    }
}
