/* Driver 2: Find_TexID */
typedef struct { char pad[0xE]; unsigned short num_polys; char pad2[4]; unsigned char *poly_block; } MODEL;
extern int D_00089F1C[];

int Find_TexID(MODEL *model, int t_id) {
    unsigned char *polylist = model->poly_block;
    int i;
    for (i = 0; i < model->num_polys; i++) {
        switch (*polylist & 0x1F) {
        case 4:
            if (*(unsigned short *)(polylist + 2) == t_id)
                return 1;
            break;
        case 5:
            if (*(unsigned short *)(polylist + 2) == t_id)
                return 1;
            break;
        case 6:
            if (*(unsigned short *)(polylist + 2) == t_id)
                return 1;
            break;
        case 7:
            if (*(unsigned short *)(polylist + 2) == t_id)
                return 1;
            break;
        case 20:
            if (*(unsigned short *)(polylist + 2) == t_id)
                return 1;
            break;
        case 21:
            if (*(unsigned short *)(polylist + 2) == t_id)
                return 1;
            break;
        case 22:
            if (*(unsigned short *)(polylist + 2) == t_id)
                return 1;
            break;
        case 23:
            if (*(unsigned short *)(polylist + 2) == t_id)
                return 1;
            break;
        }
        polylist += D_00089F1C[*polylist];
    }
    return 0;
}
