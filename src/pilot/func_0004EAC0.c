struct V3 { int x, y, z; };
void ApplyRotMatrixLV(struct V3 *, struct V3 *);
void func_0004EAC0(struct V3 *v) {
    struct V3 t;
    ApplyRotMatrixLV(v, &t);
    v->x = t.x;
    v->y = t.y;
    v->z = t.z;
}
