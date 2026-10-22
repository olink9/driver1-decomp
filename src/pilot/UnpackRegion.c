/* Driver 2: UnpackRegion */
extern short D_0009728C[];
extern int D_000A8024[];
extern int RoadMapRegions[];
extern int D_0009727C;
extern int LoadRegionData();

void UnpackRegion(int a, int b) {
    if (D_0009728C[b] == -1) {
        if (LoadRegionData()) {
            D_0009727C++;
        }
        D_000A8024[b] = a;
        RoadMapRegions[b] = a;
    }
}
