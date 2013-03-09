#include "jansson.h"
#include "assert.h"
#include <common/cmdlib.h>
#include <common/bspfile.h>

#include "qentities.h"

#define VERTEXSIZE  7

typedef struct glpoly_s
{
    struct  glpoly_s    *next;    
    int     numverts;
    int     flags;          // for SURF_UNDERWATER
    vec_t   verts[4][VERTEXSIZE];   // variable sized (xyz s1t1 s2t2)

} glpoly_t;

typedef struct msurface_s
{
    struct  msurface_s *next;    
    glpoly_t* polys;
    dface_t *face;

} msurface_t;

msurface_t* BuildSurfaceList (dface_t *face)
{
    dedge_t *r_pedge;
    int i, lindex, lnumverts;
    glpoly_t* poly;
    msurface_t* surf;
    texinfo_t* tinfo;
    float       *vec;
    float       s, t;

    tinfo = &texinfo[face->texinfo];

    surf = malloc(sizeof(msurface_t));
    memset(surf, 0, sizeof(msurface_t));

    surf->face = face;

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
            r_pedge = &dedges[lindex];
            vec = (float *) dvertexes[r_pedge->v[0]].point;
        }
        else
        {
            r_pedge = &dedges[-lindex];
            vec = (float *) dvertexes[r_pedge->v[1]].point;
        }

        s = DotProduct (vec, (float *) tinfo->vecs[0]) + tinfo->vecs[0][3];

        // FIXME once we go for textures
        s /= 64;//tinfo->texture->width;

        t = DotProduct (vec, (float *) tinfo->vecs[1]) + tinfo->vecs[1][3];

        // FIXME once we go for textures
        t /= 64;//tinfo->texture->height;

        poly->verts[i][0] = vec[0];
        poly->verts[i][1] = vec[1];
        poly->verts[i][2] = vec[2];

        poly->verts[i][3] = s;
        poly->verts[i][4] = t;

        //printf("%i %i %f, %f, %f\n", r_pedge->v[0], r_pedge->v[1], poly->verts[i][0], poly->verts[i][1], poly->verts[i][2]);

    }

    //
    // remove co-linear points 
    //

    int keeptjunctions = 0;

    #define SURF_UNDERWATER     0x80
    if (!keeptjunctions) // && !(tinfo->flags & SURF_UNDERWATER) )
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
    
    for (i = 0; i < model->numfaces; i++)
    {        
        texinfo_t* tinfo = &texinfo[face->texinfo];
        if (tinfo->flags)
        {
            face++;
            continue;
        }
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
            int ii = 0;
            int ij = i + 1;
            int ik = i + 2;

            if (false)//s->face->side)
            {
                ii = i + 2;

                ik = 0;

            }

            json_array_append_new(jvertices, json_real((int)polys->verts[ii][0]));
            json_array_append_new(jvertices, json_real((int)polys->verts[ii][1]));
            json_array_append_new(jvertices, json_real((int)polys->verts[ii][2]));

            json_array_append_new(jvertices, json_real((int)polys->verts[ij][0]));
            json_array_append_new(jvertices, json_real((int)polys->verts[ij][1]));
            json_array_append_new(jvertices, json_real((int)polys->verts[ij][2]));

            json_array_append_new(jvertices, json_real((int)polys->verts[ik][0]));
            json_array_append_new(jvertices, json_real((int)polys->verts[ik][1]));
            json_array_append_new(jvertices, json_real((int)polys->verts[ik][2]));

        }

        s = s->next;
    }

    entity_t* e;
    epair_t* epair;

    json_t* jlights = json_array();

    json_object_set(jworld, "lights", jlights);

    for (i = 0; i < num_entities; i++)
    {
        e = &entities[i];
        if (strcmp(e->classname, "light"))
            continue;            

        float x, y, z;
        float light;

        epair = e->epairs;
        while(epair) {        

            if (!strcmp(epair->key, "origin"))
            {   
                sscanf(epair->value, "%f %f %f", &x, &y, &z);
            }

            if (!strcmp(epair->key, "light"))
            {   
                sscanf(epair->value, "%f", &light);
            }
                
            epair  = epair->next;
        }

        printf("%f %f %f %f\n", x, y, z, light);

        json_array_append_new(jlights, json_integer((int)x));
        json_array_append_new(jlights, json_integer((int)y));
        json_array_append_new(jlights, json_integer((int)z));
        json_array_append_new(jlights, json_integer((int)light));

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
