
#ifndef VIEWER_ETUDIANT_H
#define VIEWER_ETUDIANT_H

#include "Viewer.h"

class ViewerEtudiant : public Viewer
{
public:
    ViewerEtudiant();

    int init();
    int render();
    int update( const float time, const float delta );

protected:
    
    /* -----------------------------------------
     Pour creer un nouvel objet vous devez :
     
     1. declarer ici dans le fichier Viewer_etudiant.h
     le Mesh,
     la texture si besoin,
     une fonction 'init_votreObjet'
     une fonction 'draw_votreObjet(const Transform& T)'
     une fonction 'draw_votreObjet(const Transform& T, unsigned int indice_texture)'

     
     --- Rq : regarder comment cela est effectue dans le fichier Viewer.h
     
     
     2. Appeler la fonction 'init_votreObjet' dans la fonction 'init' du Viewer_etudiant.cpp
     --- Rq : regarder comment cela est effectue dans le fichier Viewer.cpp
     
     
     3. Appeler la fonction 'draw_votreObjet' dans la fonction 'render' du Viewer_etudiant.cpp
     --- Rq : regarder comment cela est effectue dans le fichier Viewer.cpp
     
      ----------------------------------------- */
    
    
    /// Declaration des Mesh
    Mesh m_cylindre;
    Mesh m_cone;
    Mesh m_sphere;
    Mesh m_terrain;
    Mesh m_abatJour;
    Mesh m_disque;
    Mesh m_bilboard;
    Mesh m_cubemap;
    Mesh m_donut;
    Mesh m_triangular_prism;

    /// Declaration des Textures
    GLuint m_cylindre_tex;
    GLuint m_cone_tex;
    GLuint m_sphere_tex;
    GLuint m_tree_tex;
    GLuint m_terr_tex;
    GLuint m_cubemap_tex;
    GLuint m_donut_tex;
    GLuint m_window_tex;
    GLuint m_car_tex;
    GLuint m_wheel_tex;
    GLuint m_roof_tex;
    GLuint m_maison_text;
    GLuint m_door_tex;
    GLuint m_windowH_tex;
    GLuint m_flowerBox_tex;
    GLuint m_flower_tex;
    GLuint m_abatJour_tex;

    //Declaration des Images
    Image m_terrainAlti;
    
    /// Declaration des fonction de creation de Mesh du type init_votreObjet()
    void init_cylindre();
    void init_cone();
    void init_sphere();
    void init_abatJour();
    void init_terrain(const Image& im);
    void init_disque();
    void init_bilboard();
    void init_cubemap();
    void init_donut();
    void init_triangular_prism();

    /// Declaration des fonctions draw_votreObjet(const Transform& T)
    void draw_cylindre(const Transform &T, unsigned int tex);
    void draw_cone(const Transform& T,unsigned int tex);
    void draw_sphere(const Transform &T, unsigned int tex);
    void draw_abatJour(const Transform &T);
    void draw_avion(const Transform & T);
    void draw_snowMan(const Transform & T);
    void draw_terrain(const Transform &T,unsigned int tex);
    void draw_disque(const Transform &T, unsigned int tex);
    void draw_bilboard(const Transform &T, unsigned int tex);
    void draw_cubemap(const Transform &T, unsigned int tex);
    void draw_donut(const Transform &T, unsigned int tex);
    void draw_lake(const Transform &T);
    void draw_car(const Transform &T);
    void draw_triangular_prism(const Transform &T);
    void draw_maison(const Transform &T);
    void airplane_on_path(const Transform& T, float time);

    Vector terrainNormal(const Image& im,const int i,const int j);

    float m_time;
    Transform selfRotate(const Transform& T, float time, float speed, char axis );
    void car_on_path(const Transform& T);

    
};



#endif
