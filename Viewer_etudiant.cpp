
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "draw.h"        // pour dessiner du point de vue d'une camera
#include "Viewer_etudiant.h"

using namespace std;




/*
 * Exemple de definition de fonction permettant la creation du Mesh de votre Objet.
 */
//void ViewerEtudiant::init_votreObjet()
//{
//}


/*
 * Fonction dans laquelle les initialisations sont faites.
 */
int ViewerEtudiant::init()
{
    Viewer::init();
    
    m_camera.lookat( Point(30,50,-30), 250 );
    
    //image 
    m_terrainAlti = read_image("data/terrain/terrain.png");
    
    /// Appel des fonctions init_votreObjet pour creer les Mesh
    init_cylindre();
    init_cone();
    init_sphere();
    init_disque();
    init_donut();
    init_triangular_prism();
    init_abatJour();
    init_terrain(m_terrainAlti);
    
    init_bilboard();
    init_cubemap();
    

    /// Chargement des textures
    m_window_tex = read_texture(0, smart_path("data/cubemap/windowF.png"));
    m_car_tex =  read_texture(0, smart_path("data/cubemap/car.png"));
    m_wheel_tex = read_texture(0, smart_path("data/wheel.png"));

    m_roof_tex = read_texture(0, smart_path("data/roof.png"));

    m_cylindre_tex = read_texture(0, smart_path("data/cylinder.png"));
    m_cone_tex = read_texture(0, smart_path("data/cone.png") );
    m_sphere_tex = read_texture(0, smart_path("data/monde.png"));
    m_tree_tex = read_texture(0,smart_path("data/billboard/tree2.png") );
    m_terr_tex = read_texture(0,smart_path("data/terrain/rock.png") );
    m_cubemap_tex = read_texture(0,smart_path("data/cubemap/skybox.png") );
    m_donut_tex = read_texture(0, smart_path("data/swiming.png"));
    m_maison_text = read_texture(0, smart_path("data/curb_cub.png"));
    m_roof_tex = read_texture(0, smart_path("data/roof.png"));
    m_door_tex = read_texture(0, smart_path("data/door.png"));
    m_windowH_tex = read_texture(0, smart_path("data/windowHom.png"));
    m_flowerBox_tex = read_texture(0, smart_path("data/flowerbox.png"));
    m_flower_tex = read_texture(0, smart_path("data/flower.png"));
    m_abatJour_tex = read_texture(0, smart_path("data/abatjour.png"));
    return 0;
}

/*
 * Exemple de definition de fonction permettant l affichage
 * de 'votreObjet' subissant la Transform T
 */
//void ViewerEtudiant::draw_votreObjet(const Transform& T)
//{
// gl.texture( tex );
// gl.model( T );
// gl.draw( m_votreObjet );

// ou directement 
// draw(m_votreObjet, T, m_camera, tex);
//}
/*------------------------ CYLINDRE ----------------------------------*/

// initialisation du cylindre (maillage triangulaire par subdivision angulaire)
void ViewerEtudiant::init_cylindre(){

    int div = 25.0;
    float step = 2 * M_PI / div ;
    m_cylindre = Mesh(GL_TRIANGLES);

    // construction du cylindre en reliant les segments
    for(int i=0 ; i<div ; i++){
        float alpha = i * step;
        float alpha2 = (i+1) * step;

        float u0 = (float)i/div;
        float u1 = (float)(i+1)/div;

        // définition des sommets, normales et coordonnées texture
        m_cylindre.normal( Vector(cos(alpha), 0, sin(alpha)) );
        m_cylindre.texcoord(u0,1.0);
        int v0 = m_cylindre.vertex( Point(cos(alpha), 1, sin(alpha)) ); 

        m_cylindre.normal( Vector(cos(alpha), 0, sin(alpha)) );
        m_cylindre.texcoord(u0,0.0);
        int v1 = m_cylindre.vertex( Point(cos(alpha), -1, sin(alpha)) ); 

        m_cylindre.normal( Vector(cos(alpha2), 0, sin(alpha2)) );
        m_cylindre.texcoord(u1,1.0);
        int v2 = m_cylindre.vertex( Point(cos(alpha2), 1, sin(alpha2)) ); 

        m_cylindre.normal( Vector(cos(alpha2), 0, sin(alpha2)) );
        m_cylindre.texcoord(u1,0.0);
        int v3 = m_cylindre.vertex( Point(cos(alpha2), -1, sin(alpha2)) ); 

        // création des faces (2 triangles)
        m_cylindre.triangle(v0,v2,v1);
        m_cylindre.triangle(v1,v2,v3);
    }
}

// affichage du cylindre avec transformation
void ViewerEtudiant::draw_cylindre(const Transform &T, unsigned int tex){
    gl.model(T);
    gl.texture(tex);
    gl.draw(m_cylindre);
}


/*------------------------ CONE ----------------------------------*/

// initialisation du cône avec un triangle strip
void ViewerEtudiant::init_cone(){
    int div = 25;
    float step = (2 * M_PI) / div;

    m_cone = Mesh(GL_TRIANGLE_STRIP);

    // génération des sommets du cercle et du sommet du cône
    for (int i=0 ; i<=div ; i++){

        float alpha = i * step;
        
        // base du cône
        m_cone.normal(Vector(cos(alpha)/sqrt(2.0), 1.0/sqrt(2.0), sin(alpha)/sqrt(2.0)) );
        m_cone.texcoord(cos(alpha),0);
        m_cone.vertex(Point(cos(alpha), 0, sin(alpha)) );

        // sommet
        m_cone.normal(Vector(cos(alpha)/sqrt(2.0), 1.0/sqrt(2.0), sin(alpha)/sqrt(2.0)) );
        m_cone.texcoord(0.5,1.0);
        m_cone.vertex(Point(0, 1, 0) );        
    }
}

// affichage du cône
void ViewerEtudiant::draw_cone(const Transform &T, unsigned int tex){
    gl.model(T);
    gl.texture(tex);
    gl.draw(m_cone);
}


/*------------------------ SPHERE ----------------------------------*/

// initialisation de la sphère (paramétrisation avec angles)
void ViewerEtudiant::init_sphere(){
    m_sphere = Mesh(GL_TRIANGLES);
    float div = 25.0;

    float delta2 = (2.0* M_PI ) / div;

    // double boucle pour générer la surface
    for(float alpha = -M_PI/2.0 ; alpha< M_PI/2.0 ; alpha+=delta2 ){
        float alpha2 = alpha + delta2 ;
       
        for(float beta =0 ; beta < 2.0*M_PI ; beta+= delta2){
            float beta2 = beta + delta2;
            
            float ux0 = (float)beta / (2.0 * M_PI);
            float ux1 = (float)beta2 / (2.0 * M_PI);

            float vx0 = (float)(M_PI/2 + alpha ) / M_PI;
            float vx1 = (float)(M_PI/2 + alpha2) / M_PI;
            
            // définition des sommets avec normales et coordonnées texture
            m_sphere.normal(Vector(cos(alpha2)*cos(beta), sin(alpha2), cos(alpha2)*sin(beta)));
            m_sphere.texcoord(ux0,vx1);
            int v0 = m_sphere.vertex(Point(cos(alpha2)*cos(beta), sin(alpha2), cos(alpha2)*sin(beta)));
            
            m_sphere.normal(Vector(cos(alpha)*cos(beta), sin(alpha), cos(alpha)*sin(beta)));
            m_sphere.texcoord(ux0,vx0);
            int v1 = m_sphere.vertex(Point(cos(alpha)*cos(beta), sin(alpha), cos(alpha)*sin(beta))); 

            m_sphere.normal(Vector(cos(alpha2)*cos(beta2), sin(alpha2), cos(alpha2)*sin(beta2)));
            m_sphere.texcoord(ux1,vx1);
            int v2 = m_sphere.vertex(Point(cos(alpha2)*cos(beta2), sin(alpha2), cos(alpha2)*sin(beta2)));

            m_sphere.normal(Vector(cos(alpha)*cos(beta2), sin(alpha), cos(alpha)*sin(beta2)));
            m_sphere.texcoord(ux1,vx0);
            int v3 = m_sphere.vertex(Point(cos(alpha)*cos(beta2), sin(alpha), cos(alpha)*sin(beta2)));

            // création des faces
            m_sphere.triangle(v0,v2,v1);
            m_sphere.triangle(v1,v2,v3);
        }
    }
}

// affichage de la sphère
void ViewerEtudiant::draw_sphere(const Transform &T, unsigned int tex){
    gl.model(T);
    gl.texture(tex);
    gl.draw(m_sphere);
}

/*-----------------------------Triangular Prism --------------------------------*/
// Création d’un prisme triangulaire (2 triangles + 3 faces rectangulaires)
void ViewerEtudiant::init_triangular_prism(){
    m_triangular_prism = Mesh(GL_TRIANGLES);

    // sommets du prisme
    Point v[6]{
     { 1, -1,  1},
     { 0,  1,  1},
     {-1, -1,  1},
     
     { 1, -1, -1},
     { 0,  1, -1},
     {-1, -1, -1}, 
    };

    // triangle avant
    m_triangular_prism.normal(Vector(0,0,1));

    m_triangular_prism.texcoord(1,0);
    int v0= m_triangular_prism.vertex(v[0]);

    m_triangular_prism.texcoord(0.5,0.5);
    int v1= m_triangular_prism.vertex(v[1]);

    m_triangular_prism.texcoord(0,0);
    int v2= m_triangular_prism.vertex(v[2]);
    m_triangular_prism.triangle(v0,v1,v2);

    // triangle arrière
    m_triangular_prism.normal(Vector(0,0,1));

    m_triangular_prism.texcoord(1,0);
    int v3= m_triangular_prism.vertex(v[3]);

    m_triangular_prism.texcoord(0.5,0.5);
    int v4= m_triangular_prism.vertex(v[4]);

    m_triangular_prism.texcoord(0,0);
    int v5= m_triangular_prism.vertex(v[5]);
    m_triangular_prism.triangle(v3,v5,v4);
    
    // face rectangle 1
    m_triangular_prism.normal(Vector(1,1,1));

    m_triangular_prism.texcoord(0,0);
    v0= m_triangular_prism.vertex(v[0]);

    m_triangular_prism.texcoord(0,1);
    v1= m_triangular_prism.vertex(v[1]);

    m_triangular_prism.texcoord(1,0);
    v3= m_triangular_prism.vertex(v[3]);

    m_triangular_prism.texcoord(1,1);
    v4= m_triangular_prism.vertex(v[4]);
    
    m_triangular_prism.triangle(v0,v3,v1);
    m_triangular_prism.triangle(v3,v4,v1);

    // face rectangle 2
    m_triangular_prism.normal(Vector(0,-1,0));

    m_triangular_prism.texcoord(0,0);
    v0= m_triangular_prism.vertex(v[0]);

    m_triangular_prism.texcoord(0,1);
    v2= m_triangular_prism.vertex(v[2]);

    m_triangular_prism.texcoord(1,0);
    v3= m_triangular_prism.vertex(v[3]);

    m_triangular_prism.texcoord(1,1);
    v5= m_triangular_prism.vertex(v[5]);
    
    m_triangular_prism.triangle(v3,v0,v5);
    m_triangular_prism.triangle(v5,v0,v2);

    // face rectangle 3
    m_triangular_prism.normal(Vector(-1,1,1));

    m_triangular_prism.texcoord(1,1);
    v1= m_triangular_prism.vertex(v[1]);

    m_triangular_prism.texcoord(1,0);
    v2= m_triangular_prism.vertex(v[2]);

    m_triangular_prism.texcoord(0,1);
    v4= m_triangular_prism.vertex(v[4]);

    m_triangular_prism.texcoord(0,0);
    v5= m_triangular_prism.vertex(v[5]);

    m_triangular_prism.triangle(v1,v5,v2);
    m_triangular_prism.triangle(v4,v5,v1);
}

// affichage du prisme avec texture
void ViewerEtudiant::draw_triangular_prism(const Transform &T){
    gl.model(T);
    gl.texture(m_roof_tex);
    gl.draw(m_triangular_prism);
}


/*-------------------------------- ABAT JOUR ----------------------------------*/
// Création d’un abat-jour (surface circulaire ouverte)
void ViewerEtudiant::init_abatJour(){

    int div = 25.0;
    float step = 2 * M_PI / div ;
    m_abatJour = Mesh(GL_TRIANGLES);

    for(int i=0 ; i<div ; i++){
        float alpha = i * step;
        float alpha2 = (i+1) * step;

        float u0 = (float)i/div;
        float u1 = (float)(i+1)/div;

        // calcul des 4 points d’un segment
        Point p0 = Point(cos(alpha), 3, sin(alpha));
        Point p1 = Point(3*cos(alpha), 0, 3*sin(alpha));
        Point p2 = Point(cos(alpha2), 3, sin(alpha2)); 
        Point p3 = Point(3*cos(alpha2), 0, 3*sin(alpha2));

        // calcul de la normale
        m_abatJour.normal(normalize(cross(p0-p1, p3-p0)) );
        
        // création des sommets + UV
        m_abatJour.texcoord(u0,1.0);
        int v0 = m_abatJour.vertex(p0); 
        
        m_abatJour.texcoord(u0,0.0);
        int v1 = m_abatJour.vertex(p1); 

        m_abatJour.texcoord(u1,1.0);
        int v2 = m_abatJour.vertex(p2) ; 

        m_abatJour.texcoord(u1,0.0);
        int v3 = m_abatJour.vertex(p3); 

        // création de deux triangles
        m_abatJour.triangle(v0,v2,v1);
        m_abatJour.triangle(v1,v2,v3);
    }
}

// affichage de l’abat-jour
void ViewerEtudiant::draw_abatJour(const Transform &T){
    gl.model(T);
    gl.texture(m_abatJour_tex);
    gl.draw(m_abatJour);

    // support cylindrique (hors forme de base)
    gl.model(T * Scale(0.5,2,0.5) * Translation(0,-1,0));
    m_cylindre.default_color(Color(0,0,1));
    gl.draw(m_cylindre);
}
 /*------------------------------- AVION ------------------------------------*/
void ViewerEtudiant::draw_avion(const Transform & T){
    // aile 1
    gl.model(T * Translation(0, 0, 1.50) * Scale(1, 0.05, 2));
    gl.texture(0);
    gl.draw(m_cube);

    // aile 2
    gl.model(T * Translation(0, 0, -1.50) * Scale(1, 0.05, 2));
    gl.texture(0);
    gl.draw(m_cube);

    // corps
    gl.model(T * Scale(4,1,1));
    gl.texture(0);
    m_sphere.default_color(Color(0.8,0.5,0));
    gl.draw(m_sphere);

    // moteur 1
    gl.model(T * Translation(0, -0.5, 2) * Scale(1.5,0.5,0.3));
    gl.texture(0);
    m_sphere.default_color(Color(0,0,0));
    gl.draw(m_sphere);

    // moteur 2
    gl.model(T * Translation(0, -0.5, -2) * Scale(1.5,0.5,0.3));
    gl.texture(0);
    m_sphere.default_color(Color(0,0,0));
    gl.draw(m_sphere);

    // dérive / top
    gl.model(T * Translation(-2.5, 0, 0) * Scale(0.8, 2.5, 0.1));
    gl.texture(0);
    m_cone.default_color(Color(0,0,0));
    gl.draw(m_cone);
    
    // fenêtres
    for (int i = 0; i < 9; i += 2){
        float step = 2.0 - float(i * 3.0) / 7.0;

        gl.model(T * Translation(step, 0.3, 1) * RotationX(90) * Scale(0.2, 0.2, 0.2));
        gl.texture(0);
        m_disque.default_color(Color(0,0,1));
        gl.draw(m_disque);

        gl.model(T * Translation(step, 0.3, -1) * RotationX(-90) * Scale(0.2, 0.2, 0.2));
        gl.texture(0);
        m_disque.default_color(Color(0,0,1));
        gl.draw(m_disque);
    }
}

/*------------------------------- SNOW MAN ------------------------------------*/
void ViewerEtudiant::draw_snowMan(const Transform & T){
    //body
        gl.model(T * Scale(2,2,2));
        gl.texture(0);
        gl.draw(m_sphere);

    //face
        gl.model(T * Translation(0,3,0));
        gl.texture(0);
        gl.draw(m_sphere);

    //nose
        gl.model(T * Translation(0,3,1) * Scale(0.25,0.25, 0.75) * RotationX(90.0));
        m_cone.default_color(Color(1,0.5,0));
        gl.draw(m_cone);

    //eyes     
        gl.model(T * Translation(0.2,3.5,1) * Scale(0.1,0.1,0.1));
        gl.texture(0);
        gl.draw(m_sphere); 
        
        gl.model(T * Translation(-0.2,3.5,1) * Scale(0.1,0.1,0.1));
        gl.texture(0);
        gl.draw(m_sphere);
        

}
/*---------------------------------- DONUT -----------------------------------*/
// Création d’un donut 
void ViewerEtudiant::init_donut(){
    m_donut = Mesh(GL_TRIANGLES);
    float r = 1;
    float R = 3;
    float n_alpha=30; float n_beta=20;

    float stepa = (2 * M_PI)/ n_alpha ;
    float stepb = (2 * M_PI)/ n_beta ;

    for(float alpha =0 ; alpha< 2*M_PI ; alpha+= stepa ){
        float alpha2 = alpha + stepa ; 
        for(float beta=0; beta<2*M_PI ; beta+=stepb){

            float beta2 = beta + stepb ; 

            // calcul des 4 points du donut
            Point p0( (R + r*cos(beta2)) * cos(alpha),
                        r * sin(beta2),
                        (R + r*cos(beta2)) * sin(alpha)
                        ); 

            Point p1( (R + r*cos(beta)) * cos(alpha),
                        r * sin(beta),
                        (R + r*cos(beta)) * sin(alpha)
                        ); 

            Point p2( (R + r*cos(beta2)) * cos(alpha2),
                        r * sin(beta2),
                        (R + r*cos(beta2)) * sin(alpha2)
                        );  

            Point p3( (R + r*cos(beta)) * cos(alpha2),
                        r * sin(beta),
                        (R + r*cos(beta)) * sin(alpha2)
                        );

            // normales
            Vector n0( cos(beta2) * cos(alpha),
                        sin(beta2),
                        cos(beta2) * sin(alpha)
            );

            Vector n1( cos(beta) * cos(alpha),
                        sin(beta),
                        cos(beta) * sin(alpha)
            ); 

            Vector n2( cos(beta2) * cos(alpha2),
                        sin(beta2),
                        cos(beta2) * sin(alpha2)
            );
            
            Vector n3( cos(beta) * cos(alpha2),
                        sin(beta),
                        cos(beta) * sin(alpha2)
            );

            // coordonnées UV
            float ux0 = alpha / (2*M_PI);       
            float vx0 = beta / (2*M_PI);
            float ux1 = alpha2 / (2*M_PI);       
            float vx1 = beta2 / (2*M_PI);
            
            // sommets
            m_donut.normal(n0); 
            m_donut.texcoord(ux0,vx1); 
            int v0 = m_donut.vertex(p0);

            m_donut.normal(n1);
            m_donut.texcoord(ux0,vx0);  
            int v1 = m_donut.vertex(p1);

            m_donut.normal(n2); 
            m_donut.texcoord(ux1,ux1);
            int v2 = m_donut.vertex(p2);

            m_donut.normal(n3);
            m_donut.texcoord(ux1,ux0); 
            int v3 = m_donut.vertex(p3);
                                              
            // triangles
            m_donut.triangle(v1,v0,v2);
            m_donut.triangle(v3,v1,v2);

        }
    }
}

// affichage du tore
void ViewerEtudiant::draw_donut(const Transform& T, unsigned int tex){
    gl.model(T);
    gl.texture(tex);
    gl.draw(m_donut);
}
/*------------------------------- DISQUE  ------------------------------------*/

// Création d’un disque par subdivision angulaire
void ViewerEtudiant::init_disque(){
    m_disque = Mesh(GL_TRIANGLES);
    float div = 25.0;

    float delta2 = (2.0* M_PI ) / div;

    for(float beta =0 ; beta < 2.0*M_PI ; beta+= delta2){
        float beta2 = beta + delta2;

        // points d’un secteur
        Point p0(cos(beta),0,sin(beta));
        Point p1(0, 0, 0);
        Point p2(cos(beta2), 0, sin(beta2));

        // normale du disque
        m_disque.normal(Vector(0,1,0));

        // coordonnées texture
        m_disque.texcoord(0.5 * cos( beta)+0.5,0.5 * sin(beta )+0.5);
        int v0 = m_disque.vertex(p0);

        m_disque.texcoord(0.5,0.5);
        int v1 = m_disque.vertex(p1);

        m_disque.texcoord(0.5 * cos( beta2)+0.5,0.5 * sin(beta2 )+0.5);
        int v2 = m_disque.vertex(p2);

        // triangle du disque
        m_disque.triangle(v0,v1,v2);
    }
}

// affichage du disque
void ViewerEtudiant::draw_disque(const Transform &T, unsigned int tex ){
    gl.model(T);
    gl.texture(tex);
    gl.draw(m_disque);
}
/*---------------------------------- TERRAIN -------------------------------------*/

// Calcule la normale du terrain au point (i, j) à partir de la heightmap.
// On approxime le gradient avec les voisins (gauche/droite et haut/bas),
// puis on fait le produit vectoriel pour obtenir la normale.
Vector ViewerEtudiant::terrainNormal(const Image& im,const int i,const int j){
    int ip = i-1 ; // indice précédent en x
    int is = i+1 ; // indice suivant en x
    int jp = j-1 ; // indice précédent en z
    int js = j+1 ; // indice suivant en z

    // Points voisins (on utilise la composante rouge comme hauteur)
    Point a( ip, im(ip, j).r, j );  
    Point b( is, im(is, j).r, j );
    Point c( i, im(i, jp).r, jp );
    Point d( i, im(i, js).r, js );

    // Vecteurs directionnels
    Vector ab = normalize(b - a);
    Vector cd = normalize(d - c);

    // Produit vectoriel pour obtenir la normale
    Vector n = cross(cd,ab);

    return n; 
}

// Initialise le mesh du terrain à partir d'une image (heightmap).
// Chaque pixel correspond à un point dont la hauteur est donnée par la couleur bleu.
//(n’import laquelle, car l’image est gris donc r=g=b).
// On construit des quads transformés en 2 triangles.
void ViewerEtudiant ::init_terrain(const Image& im){
    m_terrain = Mesh(GL_TRIANGLES);
    float W = im.width()-1;   // largeur normalisée
    float H = im.height()-1;  // hauteur normalisée

    for (int i=0 ; i < im.width()-1 ; i++){
        for (int j=0; j<im.height()-1 ; j++){

            // Définition des 4 sommets du quad
            Point p0 (i, im(i, j+1).b, j+1);
            Point p1 (i, im(i, j).b, j);
            Point p2 (i+1, im(i+1, j+1).b, j+1);
            Point p3 (i+1, im(i+1, j).b, j);

            // Coordonnées de texture (UV)
            float ux0 = (float)i/W;
            float ux1 =(float)(i+1)/W;
            float vx0 = (float)j/H;
            float vx1 =(float)(j+1)/H;

            // Ajout des sommets avec normales et UV
            m_terrain.normal(terrainNormal(im, i, j+1));
            m_terrain.texcoord(ux0, vx1);
            int v0 = m_terrain.vertex(p0);

            m_terrain.normal(terrainNormal(im, i, j));
            m_terrain.texcoord(ux0, vx0);
            int v1 = m_terrain.vertex(p1);

            m_terrain.normal(terrainNormal(im, i+1, j+1));
            m_terrain.texcoord(ux1, vx1);
            int v2 = m_terrain.vertex(p2);

            m_terrain.normal(terrainNormal(im, i+1, j));
            m_terrain.texcoord(ux1, vx0);
            int v3 = m_terrain.vertex(p3);
            
            // Création des deux triangles du quad
            m_terrain.triangle(v0, v2, v1);
            m_terrain.triangle(v1, v2, v3);
        }
    }
}

// Dessine le terrain avec une transformation et une texture donnée.
void ViewerEtudiant::draw_terrain(const Transform &T, unsigned int tex){
    gl.model( T );           // applique la transformation
    gl.alpha_texture(0);     // désactive transparence
    gl.texture(tex);         // bind la texture
    gl.draw( m_terrain );    // rendu du mesh
}
/*-------------------------------- LAKE ------------------------------------*/
void ViewerEtudiant::draw_lake(const Transform &T){
    //lake
        float x = m_terrainAlti.width();
        float z = m_terrainAlti.height();
        float y = 40;

        gl.model(T);
        m_disque.default_color(Color(0,0,1));
        gl.draw(m_disque);
}

/*----------------------------- MAISON --------------------------------------*/

void ViewerEtudiant::draw_maison(const Transform &T){

    //Roof
    draw_triangular_prism(T * Translation(0, 36, 0) * Scale(15 , 4, 12));
    
    //Wall
    gl.model(T * Translation(0, 20, 0) * Scale(15 , 12, 10));
    gl.texture(m_maison_text);
    gl.draw(m_cube);

    //Chimney
    gl.model(T * Translation(5, 42, -3) * Scale(1 , 4, 1));
    gl.texture(m_roof_tex);
    gl.draw(m_cube);

    //Door
    gl.model(T * Translation(10, 14, 10.1) * Scale(6 , 6, 1));
    gl.texture(m_door_tex);
    gl.draw(m_quad);

    //window
    gl.model(T * Translation(-5, 22, 10.1) * Scale(6 , 4, 1));
    gl.texture(m_windowH_tex);
    gl.draw(m_quad);

    gl.model(T * Translation(-15.5, 22, 0) * Scale(1 , 6, 8)* RotationY(-90));
    gl.texture(m_windowH_tex);
    gl.draw(m_quad);

    //Flower box
    gl.model(T * Translation(-16.5, 16, 0) * Scale(1.3 , 2, 7)* RotationY(-90));
    gl.texture(m_flowerBox_tex);
    gl.draw(m_cube);

    for(int i = -2 ; i<3 ; i++){
        draw_bilboard(T * Translation(-16.5, 20, 2*i) * Scale(4 ,4, 4)* RotationY(-90), m_flower_tex );
    }
    

}

/* -------------------------------- TREE -----------------------------------*/

// Initialise un billboard : un simple quad vertical texturé.
// Il servira à représenter un arbre avec une image 2D.
void ViewerEtudiant::init_bilboard(){
    m_bilboard = Mesh(GL_TRIANGLES);

    // Les 4 sommets du quad
    Point p0 (-1, -1, 0);
    Point p1 (-1, 1, 0);
    Point p2 (+1, +1, 0);
    Point p3 (+1, -1, 0);

    // Normale commune au quad
    m_bilboard.normal(0, 0, 1);

    // Coordonnées de texture + ajout des sommets
    m_bilboard.texcoord(0,0);
    int v0 = m_bilboard.vertex(p0);
    
    m_bilboard.texcoord(0,1);
    int v1 = m_bilboard.vertex(p1);
    
    m_bilboard.texcoord(1,1);
    int v2 = m_bilboard.vertex(p2);
    
    m_bilboard.texcoord(1,0);
    int v3 = m_bilboard.vertex(p3);
    
    // Le quad est découpé en 2 triangles
    m_bilboard.triangle(v0,v2,v1);
    m_bilboard.triangle(v2,v0,v3);
}

// Dessine plusieurs billboards croisés pour donner du volume à l’arbre.
// On fait tourner le même quad autour de l’axe Y.
void ViewerEtudiant::draw_bilboard(const Transform &T, unsigned int tex){

    for (int i = 0; i< 4 ; i++){
        float alpha = 45.0;

        // Face avant
        gl.model( T *  RotationY(i * alpha));
        gl.alpha_texture(tex);
        gl.draw(m_bilboard);

        // Face arrière
        gl.model( T * RotationY(180 + (i * alpha)) );
        gl.alpha_texture(tex);
        gl.draw(m_bilboard);
    }
}
/*----------------------------------- CAR ----------------------------------- */

void ViewerEtudiant::draw_car(const Transform& T){

    float t = m_time / 1000.0f;
    float angle = 300.0f * t;   

    //body
    gl.model(T* Translation(0,1,0) *Scale(4,1.5,2));
    gl.texture(m_car_tex);
    gl.draw(m_cube);

    //window
    gl.model(T* Translation(0,3.5,0) *Scale(2,1,2));
    gl.texture(m_window_tex);
    gl.draw(m_cube);

    //wheel
    gl.model(T* Translation(1.5,-1,2.01) * RotationX(90)* RotationY(-angle));
    m_disque.default_color(Color(1,1,1));
    gl.texture(m_wheel_tex);
    gl.draw(m_disque);

    gl.model(T* Translation(1.5,-1,-2.01) * RotationX(-90) * RotationY(angle));
    gl.texture(m_wheel_tex);
    gl.draw(m_disque);

    gl.model(T* Translation(-1.5,-1,2.01) * RotationX(90) * RotationY(-angle));
    gl.texture(m_wheel_tex);
    gl.draw(m_disque);

    gl.model(T* Translation(-1.5,-1,-2.01) * RotationX(-90) * RotationY(angle));
    gl.texture(m_wheel_tex);
    gl.draw(m_disque);

    //light
    gl.model(T* Translation(4,2,1.5)* Scale(0.2,0.2,0.2));
    m_sphere.default_color(Color(1,1,0));
    gl.texture(0);
    gl.draw(m_sphere);

    gl.model(T* Translation(4,2,-1.5) * Scale(0.2,0.2,0.2));
    m_sphere.default_color(Color(1,1,0));
    gl.texture(0);
    gl.draw(m_sphere);

    gl.model(T* Translation(4,1,1.5)* Scale(0.4,0.4,0.4));
    m_sphere.default_color(Color(1,1,1));
    gl.texture(0);
    gl.draw(m_sphere);

    gl.model(T* Translation(4,1,-1.5) * Scale(0.4,0.4,0.4));
    m_sphere.default_color(Color(1,1,1));
    gl.texture(0);
    gl.draw(m_sphere);
}

/*----------------------------------- CUBEMAP ----------------------------------- */

/*
 Ce code construit et affiche un cubemap (skybox) autour de la scène.
 Le cubemap est un grand cube texturé qui entoure le terrain pour simuler
 un environnement (ciel, horizon, etc.).

 Chaque face du cube correspond à une partie de la texture (atlas),
 avec des coordonnées UV spécifiques.
*/

    //       4---5
    //      /|  /|
    //     7---6 |
    //     | 0-|-1
    //     |/  |/
    //     3---2

void ViewerEtudiant::init_cubemap(){
    m_cubemap = Mesh(GL_TRIANGLES);

    // Dimensions du cube centrées autour du terrain
    float x = m_terrainAlti.width()/2;
    float z = m_terrainAlti.height()/2;
    float y = 2 * x;

    // Définition des 8 sommets du cube
    Point pt[8]{
        {-x, 0, -z},
        { x, 0, -z},
        { x, 0,  z},
        {-x, 0,  z},

        {-x, y, -z},
        { x, y, -z},
        { x, y,  z},
        {-x, y,  z}
    };

    // Indices des sommets pour chaque face du cube
    int f[6][4] = {{0, 1, 2, 3}, {5, 4, 7, 6}, {2, 1, 5, 6}, {0, 3, 7, 4}, {3, 2, 6, 7}, {1, 0, 4, 5}};

    // Normales de chaque face (orientation)
    Vector n[6] = {{0, 1, 0}, {0, -1, 0}, {-1, 0, 0}, {1, 0, 0}, {0, 0, -1}, {0, 0, 1}};

    // Organisation de la texture (atlas en croix)
    // Chaque face du cube correspond à une zone de la texture

    // 3          °----°
    //            | tp |
    // 2     °----°----°----°----°
    //       |  L | fr | R  | bk |
    // 1     °----°----°----°----°
    //            | bt |
    // 0          °----°
    //v / u  0    1    2    3    4

    const float du = 1.0f / 4.0f;
    const float dv = 1.0f / 3.0f;

    // Coordonnées UV pour chaque face
    float uv[6][4] = {
        {1*du, 0*dv, 2*du, 1*dv}, // bottom
        {1*du, 2*dv, 2*du, 3*dv}, // top
        {2*du, 1*dv, 3*du, 2*dv}, // right
        {0*du, 1*dv, 1*du, 2*dv}, // left
        {3*du, 1*dv, 4*du, 2*dv}, // back
        {1*du, 1*dv, 2*du, 2*dv}  // front
          
    };

    // Construction des 6 faces du cube
    for (int i= 0 ; i <6 ;i++){

        float ux0 = uv[i][0];
        float vx0 = uv[i][1];
        float ux1 = uv[i][2];
        float vx1 = uv[i][3];

        // Ajout des sommets avec normales et UV
        m_cubemap.normal(n[i]);
        m_cubemap.texcoord(ux1,vx0);
        int v0 = m_cubemap.vertex(pt[ f[i][0] ]);

        m_cubemap.texcoord(ux0,vx0);
        int v1 = m_cubemap.vertex(pt[ f[i][1] ]);

        m_cubemap.texcoord(ux0,vx1);
        int v2 = m_cubemap.vertex(pt[ f[i][2] ]);

        m_cubemap.texcoord(ux1,vx1);
        int v3 = m_cubemap.vertex(pt[ f[i][3] ]);

        // Chaque face est composée de 2 triangles
        m_cubemap.triangle(v3,v2,v1);
        m_cubemap.triangle(v1,v0,v3);
    }
}

// Affiche le cubemap avec la transformation et la texture donnée
void ViewerEtudiant::draw_cubemap(const Transform& T, unsigned int tex){
    gl.model(T);        // applique la transformation
    gl.texture(tex);    // bind la texture du cubemap
    gl.draw(m_cubemap); // dessine le cube
}

/*----------------------- MOVE OBJ -------------------------------*/

/*
 Ces fonctions permettent d’animer des objets dans la scène.
 On applique des transformations (translation + rotation) en fonction du temps
 pour simuler des mouvements réalistes (rotation, déplacement sur trajectoire).
*/

// Applique une rotation locale à un objet autour d’un axe donné (x, y ou z)
// La vitesse dépend du temps écoulé
Transform ViewerEtudiant::selfRotate(const Transform& T, float time, float speed, char axis ){
    float t = time / 1000.0f;
    float angle = speed * t;

    if ( axis == 'x' ) return T * RotationX(angle);
    if ( axis == 'z' ) return T * RotationZ(angle);
    return T * RotationY(angle);
}

// Fait déplacer un avion le long d’un chemin défini par plusieurs points
// La position est interpolée entre deux points successifs
// L’orientation est calculée pour suivre la direction du mouvement
void ViewerEtudiant::airplane_on_path(const Transform& T, float time){
    float x = m_terrainAlti.width()/2; 
    float z = m_terrainAlti.height()/2;
    float y = 50;
    int NB = 10;

    // Points définissant la trajectoire de l’avion
    Point path[10]{
        { -80, y, -20},
        { -60, y, -60},
        { -20, y, -80},
        { 30, y, -70},
        { 70, y, -30},
        { 80, y, 20},
        { 40, y, 60},
        {  0, y, 80},
        { -50, y, 60},
        { -70, y, 20}
    };

    float t = time / 5000.0f;

    int i = ((int)t) % NB;
    int j = (i + 1) % NB;
    float u = t - (int)t;

    // Interpolation de la position entre deux points
    Point pos(
        (1-u)*path[i].x + u*path[j].x,
        (1-u)*path[i].y + u*path[j].y,
        (1-u)*path[i].z + u*path[j].z
    );

    // Direction du mouvement
    Vector IJ = normalize(path[j] - path[i]);

    Vector dir = normalize((1-u) * IJ + u * IJ);
    float angle = atan2(dir.z, dir.x) * 180.0f / M_PI;

    // Dessin de l’avion avec translation et rotation
    draw_avion(T * Translation(pos.x, pos.y, pos.z)*Scale(2,2,2) * RotationY(-angle));
}

// Fait déplacer une voiture sur un chemin en suivant le relief du terrain
void ViewerEtudiant::car_on_path(const Transform& T)
{
    const int NB = 6;

    // Points de la trajectoire
    Point path[NB]{
        {-80, 0, -40},
        {-40, 0, -20},
        {  0, 0, -10},
        { 30, 0,  20},
        { 10, 0,  60},
        {-50, 0,  40}
    };

    float t = m_time / 7000.0f;     // vitesse du déplacement
    int i = ((int)t) % NB;
    int j = (i + 1) % NB;
    float u = t - (int)t;

    // Position interpolée
    float xw = (1-u) * path[i].x + u * path[j].x;
    float zw = (1-u) * path[i].z + u * path[j].z;

    // Conversion en indices de la heightmap
    int ix = (int)(xw + m_terrainAlti.width() / 2);
    int iz = (int)(zw + m_terrainAlti.height() / 2);

    // Limitation pour rester dans les bornes de l’image
    ix = std::max(0, std::min(ix, m_terrainAlti.width() - 1));
    iz = std::max(0, std::min(iz, m_terrainAlti.height() - 1));

    // Calcul de la hauteur du terrain
    // ( * 20, car j’ai appliqué un facteur d’échelle à la hauteur du terrain
    // et j’ajoute 3 à cause du rayon des roues de la voiture.)
    float yw = 20.0f * m_terrainAlti(ix, iz).r + 3.0f;

    // Direction du mouvement
    Vector dir = normalize(path[j] - path[i]);
    float angle = atan2(dir.z, dir.x) * 180.0f / M_PI;

    // Dessin de la voiture avec adaptation au terrain
    draw_car( T * Translation(xw, yw, zw)*Scale(2,2,2) * RotationY(-angle) ) ;
}

/*
 * Fonction dans laquelle les appels pour les affichages sont effectues.
 */

int ViewerEtudiant::render()
{
    srand(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    manageCameraLight();

    gl.camera(m_camera);
    
    
    /// Appel de la fonction render() de la class Viewer
    /// pour que vous voyez apparaitre un premier objet : quad avec texture de papillon
    /// a supprimer ensuite
    Viewer::render();
    
    
    /// Appel des fonctions du type 'draw_votreObjet'
    // draw_votreObjet(const Transform& T);

    //cube
    //draw_cube(Translation(-3,1,1),m_cube_texture);
    //draw_cube(Translation(3,2,1) * Scale(1,2,1),m_cube_texture);
    //draw_cube(Translation(0,2,-3) * Scale(1,2,1) * RotationZ(45.0),m_cube_texture);

    //cylindre
    //draw_cylindre(Translation(+3, +2, -3) * Scale(1,2,1) * RotationY(40),m_cylindre_tex);

    //cone
    //draw_cone(Translation(+3, 0, 1) * RotationY(-20),m_cone_tex);

    //sphere
    //draw_sphere(Translation(-3,1,0), m_sphere_tex);

    //obj complex 
    //draw_snowMan(Translation(5,0,3));
    //draw_avion(Translation(0,5,-5));
    //draw_car(Translation(-12,0,0));
    //draw_maison(Translation(0,0,0));
    
    //d'autre obj de base
    //draw_disque(Translation(0, 10, 0)*RotationX(90),m_cylindre_tex); 
    //draw_triangular_prism(Translation(-5,0,0));
    //draw_abatJour(Translation(5,3,0));
    //draw_donut(Translation(0,0,0),m_donut_tex);

    //test de la fonct. selfRotate
    //Transform rotat = selfRotate(Translation(0,0,0), 45.0f, 'y');

    airplane_on_path(Translation(0, 0 ,0), m_time);

    //tree & terrain
    float x = m_terrainAlti.width()/2;
    float z = m_terrainAlti.height()/2;
    Transform TT =  Translation(-x,0,-z) ;

    draw_terrain(TT * Scale(1, 20 ,1) , m_terr_tex);

    for(int k = 0; k <10; k++)
    {
        int x = rand() % m_terrainAlti.width();
        int z = rand() % m_terrainAlti.height();

        float y =20 * m_terrainAlti(x, z).r;

        draw_bilboard(TT * Translation(x, y+10, z) * Scale(10, 10, 10) , m_tree_tex);
    }

    //cubemap
    draw_cubemap(Translation(0,0,0), m_cubemap_tex);

    //lake
    draw_lake(Translation(60,2.5,-10) * Scale(30,1,70));

    //swim ring
    draw_donut(Translation(50,2.5,-10)* Scale(0.5,0.5,0.5),m_donut_tex);
    
    //car 
    car_on_path(Translation(0,0,0));

    //home
    draw_maison(Translation(55,2,-50));
    return 1;
}


/*
 * Fonction dans laquelle les mises a jours sont effectuees.
 */
int ViewerEtudiant::update( const float time, const float delta )
{
    // time est le temps ecoule depuis le demarrage de l'application, en millisecondes,
    // delta est le temps ecoule depuis l'affichage de la derniere image / le dernier appel a draw(), en millisecondes.
    m_time = time;
    
    
    
    return 0;
}


/*
 * Constructeur.
 */

ViewerEtudiant::ViewerEtudiant() : Viewer()
{
}


/*
 * Programme principal.
 */
int main( int argc, char **argv )
{
    
    ViewerEtudiant v;
    v.run();
    return 0;
}
