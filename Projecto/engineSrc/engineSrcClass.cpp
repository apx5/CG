#include "engineSrcClass.h"

vector<struct modelo*> SysState::modelos;
vector<struct geo_transf*> SysState::geo_tr;
vector<struct translate*> SysState::tr;
vector<struct light*> SysState::lights;
vector<int> SysState::sequencia;
vector <float> SysState::preVBO;
vector<float> SysState::preNormal;
vector<float> SysState::preTextCoord;

extern float angleBeta,angleAlfa,distanciaCamera; // variaveis globais externas do ficheiro main.cpp
extern GLuint buffers[3]; // variaveis globais externas do ficheiro main.cpp

extern int alpha, beta;
extern float r, camX, camY, camZ, radius, sensitivity;

// constante da camara para deslocações
extern float k_X, k_Z;

// iniciar Px, Pz
extern float Px,Py,Pz;

//iniciar lookAt
extern float Lx,Lz,Ly;

extern int startX, startY, tracking;

SysState::SysState(char *fileName){
    readXML(fileName);

    glGenBuffers(3, buffers);
    //carregar os dados para o VBO
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER,preVBO.size()*sizeof(float), preVBO.data(), GL_STATIC_DRAW);

    //carregar os dados para o lig
    glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
    glBufferData(GL_ARRAY_BUFFER,preVBO.size()*sizeof(float), preNormal.data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, preTextCoord.size() * sizeof(float), &(preTextCoord[0]), GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


    // Turn on lighting and Define light color

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

    for(int i=0; i < lights.size(); i++){

      glEnable(GL_LIGHT0 + i);

      GLfloat dark[4] = {0.2, 0.2, 0.2, 1.0};
      GLfloat white[4] = {0.9, 0.9, 0.9, 0.};

      // light colors
      glLightfv(GL_LIGHT0 + i, GL_AMBIENT, dark);
      glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, white);
      glLightfv(GL_LIGHT0 + i, GL_SPECULAR, white);
    }
}

int SysState::read3D(char *filename,float diffR, float diffG, float diffB,float emiR,float emiG,float emiB,char *texfile){

  Modelo *mod =(struct modelo*) malloc(sizeof(struct modelo));

  mod->posInitVBO=0;
  mod->posInitNormal=0;

  int counterP = 0;
  int numPoints = 0;
  int validArgs = 0;

  bool erro = false;

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename,"r");
  if (fp) {

    // parse da primeira linha que contem os numeros dos pontos
    if ((read = getline(&line, &len, fp)) != -1) { 
      validArgs = sscanf(line, "%d", &numPoints);

      if (validArgs!=1){
        printf("Number of lines not defined!!\n");
        erro = true;
      }
      free(line);
      line=NULL;
    }

    if (!erro){

      mod->posInitVBO=preVBO.size()/3; // num pontos
    
      while ((counterP < numPoints) && (read = getline(&line, &len, fp)) != -1) {

        float x=0.0,y=0.0,z=0.0;
        validArgs = sscanf(line,"%f %f %f",&x,&y,&z);

        preVBO.push_back(x);
        preVBO.push_back(y);
        preVBO.push_back(z);

        if (validArgs!=3) {
          printf("Invalid point!! on line: %d (of %d)\n",counterP+1, numPoints+1);
          erro = true;
          break;
        }
        else {
          ++counterP;
          free(line);
          line=NULL;
        }
      }

      counterP=0;
      //mod->posInitNormal=pre.size()/3;

      while ((counterP < numPoints) && (read = getline(&line, &len, fp)) != -1) {

        float x=0.0,y=0.0,z=0.0;
        validArgs = sscanf(line,"%f %f %f",&x,&y,&z);

        preNormal.push_back(x);
        preNormal.push_back(y);
        preNormal.push_back(z);

        if (validArgs!=3) {
          printf("Invalid point!! on line: %d (of %d)\n",counterP+1, numPoints+1);
          erro = true;
          break;
        }
        else {
          ++counterP;
          free(line);
          line=NULL;
        }
      }

      counterP=0;

      while ((counterP < numPoints) && (read = getline(&line, &len, fp)) != -1) {

        float x=0.0,y=0.0;
        validArgs = sscanf(line,"%f %f",&x,&y);

        preTextCoord.push_back(x);
        preTextCoord.push_back(y);
        

        if (validArgs!=2) {
          printf("Invalid point!! on line: %d (of %d)\n",counterP+1, numPoints+1);
          erro = true;
          break;
        }
        else {
          ++counterP;
          free(line);
          line=NULL;
        }
      }
    }
  }
  else{
    // não abriu o ficheiro
    cout << "File \"" << filename << "\" not found " << endl;
    return 0;
  }

  fclose(fp);

  mod->numPoints = numPoints;

  mod->diffR=diffR;
  mod->diffG=diffG;
  mod->diffB=diffB;
  mod->emiR=emiR;
  mod->emiG=emiG;
  mod->emiB=emiB;

  mod->texID = 0;

  if(texfile){  
    mod->texID = loadTexture(texfile);
  }

  //cout << mod->texID << endl;

  modelos.push_back(mod);
  return 1;
}

void SysState::parserXML(TiXmlElement *element){

    //if para verificar que filho é
        //primeiro verificar se n é nulo
        while (element){

          //caso model
          if(!strcmp(element->Value(), "model")){

                char *name3D = (char *) element->Attribute("file");

                float diffR=1,diffG=1,diffB=1; 
                float emiR=0,emiG=0,emiB=0;

                element->QueryFloatAttribute("diffR",&diffR);
                element->QueryFloatAttribute("diffG",&diffG);
                element->QueryFloatAttribute("diffB",&diffB);
                element->QueryFloatAttribute("emiR",&emiR);
                element->QueryFloatAttribute("emiG",&emiG);
                element->QueryFloatAttribute("emiB",&emiB);

                char *texture=NULL;
                if(element->Attribute("texture")){
                  texture = (char *) element->Attribute("texture");
                }

                //texture = (string) element->Attribute("texture");
                //if(texture){
                  //cout << texture << endl;
                //}
                
        
                if(read3D(name3D,diffR,diffG,diffB,emiR,emiG,emiB,texture)){
                  sequencia.push_back(3);
                }
            }

          //caso translate
          if(!strcmp(element->Value(), "translate")){
            Translate *t = new Translate();
            t->time = 0;

            t->cp.clear();

            element->QueryFloatAttribute("time",&t->time);

            tr.push_back(t);
            parserXML(element->FirstChildElement());
            sequencia.push_back(4);
          }

          //caso rotate
          if(!strcmp(element->Value(), "rotate")){
            Geo_Transf *geo =(struct geo_transf*) malloc(sizeof(struct geo_transf));
            geo->angle=0;
            geo->x=0;
            geo->y=0;
            geo->z=0;
            geo->time=1;

            geo->tipo = 1;

            element->QueryFloatAttribute("time",&geo->time);
            element->QueryFloatAttribute("angle",&geo->angle);
            element->QueryFloatAttribute("axisX",&geo->x);
            element->QueryFloatAttribute("axisY",&geo->y);
            element->QueryFloatAttribute("axisZ",&geo->z);

            geo_tr.push_back(geo);

            sequencia.push_back(2);
          }

          //caso scale
          if(!strcmp(element->Value(), "scale")){

            Geo_Transf *geo =(struct geo_transf*) malloc(sizeof(struct geo_transf));
            geo->x=0;
            geo->y=0;
            geo->z=0;

            geo->tipo = 2;

            element->QueryFloatAttribute("X",&geo->x);
            element->QueryFloatAttribute("Y",&geo->y);
            element->QueryFloatAttribute("Z",&geo->z);

            geo_tr.push_back(geo);

            sequencia.push_back(2);
          }

          //caso group
          if(!strcmp(element->Value(), "group") || !strcmp(element->Value(), "lights")){

            sequencia.push_back(0);

            parserXML(element->FirstChildElement());

            sequencia.push_back(1);
          }

          //pode n estar correto 
          if(!strcmp(element->Value(), "models")){

            parserXML(element->FirstChildElement());
          }

          if(!strcmp(element->Value(), "point")){
            
            Point *p = (struct point*) malloc(sizeof(struct point));
            p->x = 0;
            p->y = 0;
            p->z = 0;
            
            element->QueryFloatAttribute("X",&p->x);
            element->QueryFloatAttribute("Y",&p->y);
            element->QueryFloatAttribute("Z",&p->z);

            tr[tr.size()-1]->cp.push_back(p);
          }

          if(!strcmp(element->Value(), "light")){
            
            Light *l = (struct light*) malloc(sizeof(struct light));
            Point *p = (struct point*) malloc(sizeof(struct point));
            Point *d = (struct point*) malloc(sizeof(struct point));
            p->x = 0;
            p->y = 0;
            p->z = 0;

            d->x = 0;
            d->y = 0;
            d->z = 0;
            
            element->QueryFloatAttribute("posX",&p->x);
            element->QueryFloatAttribute("posY",&p->y);
            element->QueryFloatAttribute("posZ",&p->z);

            element->QueryFloatAttribute("dirX",&d->x);
            element->QueryFloatAttribute("dirY",&d->y);
            element->QueryFloatAttribute("dirZ",&d->z);

            l->point = p;
            l->dir = d;
            l->angle = 90.0;
            l->quad_att = 0.0;
            l->exponent = 0.0;
            l->type = 0;

            element->QueryFloatAttribute("angle",&l->angle);
            element->QueryFloatAttribute("attenuation",&l->quad_att);
            element->QueryFloatAttribute("exponent",&l->exponent);

            const char *type;
            type = element->Attribute("type");

            if(!strcmp(type, "POINT")) l->type = 0;
            else if(!strcmp(type, "DIRECTIONAL")) l->type = 1;
            else if(!strcmp(type, "SPOT")) l->type = 2;

            lights.push_back(l);

            sequencia.push_back(5);
          }

          if(element){
              element = element->NextSiblingElement();
          }
        }
}

void SysState::readXML(char *pFilename){
    // abrir ficheiro xml
    TiXmlDocument doc( pFilename );
    bool loadOkay = doc.LoadFile();

    if (loadOkay) {
        TiXmlElement *l_pRootElement = doc.RootElement();

        TiXmlElement *element = l_pRootElement->FirstChildElement();

        if (element){
            parserXML(element);
        }

        //procuramos o proximo filho 
        element = element->FirstChildElement();  
    }
}

void SysState::renderScene(void) {

    // clear buffers
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*
    gluLookAt(sin(angleAlfa)*cos(angleBeta)*distanciaCamera, sin(angleBeta)*distanciaCamera, cos(angleAlfa)*cos(angleBeta)*distanciaCamera, 
		      	0,0,0,
			  	0.0f,1.0f,0.0f);
    */

    gluLookAt(  Px, Py, Pz, 
            Lx,Ly,Lz,
          0.0f,1.0f,0.0f);

    // put the geometric transformations here
    
    // put drawing instructions here
    writeSeq();

    // End of frame
    glutSwapBuffers();
}

void SysState::writeSeq(){
  int numGeo = 0, numModelo = 0, numTranf = 0, numLigths = 0;

  //percorrer vector sequencia
  for(int i=0;i<sequencia.size();i++){
    //verificar caso
    switch (sequencia[i]) {
      case 0:
        glPushMatrix();
        break;
      case 1:
        glPopMatrix();
        break;
      case 2:
        if(numGeo<geo_tr.size()) writeGeo(geo_tr[numGeo++]);
        break;
      case 3:
        if(numModelo<modelos.size()) writeModelo3D(modelos[numModelo++]);
        break;
      case 4:
        if(numTranf<tr.size()) writeTranslate(tr[numTranf++]);
        break;
      case 5:
        if(numLigths<lights.size()) writeLigth(lights[numLigths], numLigths);
        numLigths++;
        break;
    }
  }
}