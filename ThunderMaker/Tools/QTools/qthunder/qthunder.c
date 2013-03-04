#include "jansson.h"
#include <common/cmdlib.h>
#include <common/bspfile.h>

#include "qentities.h"

#define VERTEXSIZE  7

typedef struct glpoly_s
{
    struct  glpoly_s    *next;    
    int     numverts;
    int     flags;          // for SURF_UNDERWATER
    float   verts[4][VERTEXSIZE];   // variable sized (xyz s1t1 s2t2)

} glpoly_t;

typedef struct msurface_s
{
    struct  msurface_s *next;    
    glpoly_t* polys;

} msurface_t;

msurface_t* BuildSurfaceList (dface_t *face)
{
    dedge_t *pedges, *r_pedge;
    int i, lindex, lnumverts;
    glpoly_t* poly;
    msurface_t* surf;
    texinfo_t* tinfo;
    vec_t       *vec;
    float       s, t;

    pedges = &dedges[0];
    tinfo = &texinfo[face->texinfo];

    surf = malloc(sizeof(msurface_t));
    memset(surf, 0, sizeof(msurface_t));

    // reconstruct the polygon    
    lnumverts = face->numedges;

    poly = (glpoly_t*) malloc (sizeof(glpoly_t) + (lnumverts-4) * VERTEXSIZE*sizeof(float));
    poly->next = NULL;//fa->polys;
    poly->flags = tinfo->flags;
    surf->polys = poly;
    poly->numverts = lnumverts;

    for (i=0 ; i<lnumverts ; i++)
    {
        lindex = dsurfedges[face->firstedge + i];

        if (lindex > 0)
        {
            r_pedge = &pedges[lindex];
            vec = (vec_t *) dvertexes[r_pedge->v[0]].point;
        }
        else
        {
            r_pedge = &pedges[-lindex];
            vec = (vec_t *) dvertexes[r_pedge->v[1]].point;
        }
        s = DotProduct (vec, (vec_t *) tinfo->vecs[0]) + tinfo->vecs[0][3];

        // FIXME once we go for textures
        s /= 64;//tinfo->texture->width;

        t = DotProduct (vec, (vec_t *) tinfo->vecs[1]) + tinfo->vecs[1][3];

        // FIXME once we go for textures
        t /= 64;//tinfo->texture->height;

        VectorCopy (vec, (vec_t *) &poly->verts[i]);
        poly->verts[i][3] = s;
        poly->verts[i][4] = t;

        //printf("%f, %f, %f\n", poly->verts[0][0], poly->verts[1][1], poly->verts[2][2]);

    }

    //
    // remove co-linear points 
    //

    int keeptjunctions = 0;

    #define SURF_UNDERWATER     0x80
    if (!keeptjunctions && !(tinfo->flags & SURF_UNDERWATER) )
    {
        for (i = 0 ; i < lnumverts ; ++i)
        {
            vec3_t v1, v2;
            vec_t *prev, *this, *next;
            float f;

            prev = poly->verts[(i + lnumverts - 1) % lnumverts];
            this = poly->verts[i];
            next = poly->verts[(i + 1) % lnumverts];

            VectorSubtract( this, prev, v1 );
            VectorNormalize( v1 );
            VectorSubtract( next, prev, v2 );
            VectorNormalize( v2 );

            // skip co-linear points
            #define COLINEAR_EPSILON 0.001
            if ((fabs( v1[0] - v2[0] ) <= COLINEAR_EPSILON) &&
                (fabs( v1[1] - v2[1] ) <= COLINEAR_EPSILON) && 
                (fabs( v1[2] - v2[2] ) <= COLINEAR_EPSILON))
            {
                int j;
                for (j = i + 1; j < lnumverts; ++j)
                {
                    int k;
                    for (k = 0; k < VERTEXSIZE; ++k)
                        poly->verts[j - 1][k] = poly->verts[j][k];
                }
                --lnumverts;
                //++nColinElim;
                // retry next vertex next time, which is now current vertex
                --i;
            }
        }
    }

    poly->numverts = lnumverts;

    return surf;

}

static void process_worldspawn(entity_t* worldspawn)
{
    int i;
    vec_t *v0, *v1, *v2;

    json_t *jworld = json_object();

    epair_t* ep = worldspawn->epairs;
    while(ep) {        
        ep  = ep->next;
    }

    dmodel_t* model = &dmodels[0];
    dface_t *face = &dfaces[model->firstface];

    msurface_t* s;
    msurface_t* surfaces = NULL;

    //0,1,2 0,2,3 0,3,4 

    
    for (i = 0; i < model->numfaces; i++)
    {        
        s = BuildSurfaceList(face);
        s->next = surfaces;
        surfaces = s;
        face++;        
    }

    json_t* jvertices = json_array();

    json_object_set(jworld, "vertices", jvertices);

    int numpolys = 0;
    s = surfaces;
    while (s)
    {

        glpoly_t* polys = s->polys;

        if (polys->numverts < 3)
        {
            s = s->next;
            continue;
        }

        numpolys += s->polys->numverts - 2;

        for (i = 0; i < polys->numverts - 2; i++)
        {
            json_array_append_new(jvertices, json_real(polys->verts[0][0]));
            json_array_append_new(jvertices, json_real(polys->verts[0][1]));
            json_array_append_new(jvertices, json_real(polys->verts[0][2]));

            json_array_append_new(jvertices, json_real(polys->verts[i][0]));
            json_array_append_new(jvertices, json_real(polys->verts[i][1]));
            json_array_append_new(jvertices, json_real(polys->verts[i][2]));

            json_array_append_new(jvertices, json_real(polys->verts[i+1][0]));
            json_array_append_new(jvertices, json_real(polys->verts[i+1][1]));
            json_array_append_new(jvertices, json_real(polys->verts[i+1][2]));

        }

        s = s->next;
    }

    json_object_set(jworld, "numpolys", json_integer(numpolys));

    const char* dump = json_dumps(jworld, 0);
    FILE* file = fopen("hey.jsn", "wb");
    fwrite(dump, 1,  strlen(dump) + 1, file);
    fclose(file);
    
}

static void process() {

    int j;
    entity_t* e;
    epair_t* epair;

    LoadEntities();    

    for (j = 0; j < num_entities; j++)
    {
        e = &entities[j];
        if (!strcmp(e->classname, "worldspawn"))
            process_worldspawn(e);

        epair = e->epairs;
        while(epair) {        
            //printf("%s:%s\n", epair->key, epair->value);
            epair  = epair->next;
        }

    }

}

int main(int argc, char **argv)
{
    int i;
    char source[1024];

    if (argc == 1)
	Error("usage: bspinfo bspfile [bspfiles]");

    for (i = 1; i < argc; i++) {
	printf("---------------------\n");
	strcpy(source, argv[i]);
	DefaultExtension(source, ".bsp");
	printf("%s\n", source);

	LoadBSPFile(source);

    process();

	//PrintBSPFileSizes();
	printf("---------------------\n");
    }

    return 0;
}
