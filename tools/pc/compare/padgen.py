#!/usr/bin/env python3
"""padgen.py LENGTH [SEED] -> a D1_PADF / lrhost --padf script (game frames): accelerate (x) for 200 frames with
random steering bursts, then reverse with a turn (sq+left/right) for 35 - repeated. Crude, but it keeps the car
moving, hits things (damage), runs into traffic and cops (felony, chases). Same seed -> same script."""
import sys, random
n = int(sys.argv[1]); rnd = random.Random(int(sys.argv[2]) if len(sys.argv) > 2 else 1)
ev = []
f = 30
while f < n:
    # drive forward 200 frames with a few steering bursts
    ev.append(f"{f}-{f+199}=x")
    g = f + rnd.randint(10, 40)
    while g < f + 190:
        d = rnd.choice(["left", "right"])
        ln = rnd.randint(4, 20)
        ev.append(f"{g}-{g+ln}={d}")
        g += ln + rnd.randint(15, 60)
    f += 200
    # back out: reverse with a turn
    d = rnd.choice(["left", "right"])
    ev.append(f"{f}-{f+34}=sq+{d}")
    f += 40
print(",".join(ev))
