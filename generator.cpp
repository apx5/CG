#include "generator.h"

void print_sphere(float radius,float slices,float stacks,char* file_name){
    FILE *fd = fopen(file_name,"w");
    if (fd){
        float alfa = 2*M_PI / (slices*4);
        float beta = (M_PI/2) / stacks;
        float d = (radius/2) / stacks;


        // desenhar topo e base
        for(int i=0; i< (slices*4) ; i++){

            fprintf(fd, "glColor3f(1,0,0);\n");
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(stacks-1))*radius*sin(alfa*i),sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*i));
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(stacks-1))*radius*sin(alfa*(i+1)),sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*(i+1)));
            fprintf(fd, "glVertex3f(0,%f,0);\n",radius); // topo da esfera

            fprintf(fd, "glColor3f(0,1,0);\n");
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(stacks-1))*radius*sin(alfa*i),-sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*i));
            fprintf(fd, "glVertex3f(0,%f,0);\n",-radius); // base da esfera
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(stacks-1))*radius*sin(alfa*(i+1)),-sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*(i+1)));
        }

        if (stacks<2) stacks = 2;

        for(int j = 0; j< stacks-1;j++){ // ciclo externo beta
            for(int i = 0;i< (slices*4);i++){ // ciclo interno alfa
            // metade superior
                // primeiro trinagulo
            fprintf(fd, "glColor3f(0,0,1);\n");
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*j)*radius*sin(alfa*i),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*j)*radius*sin(alfa*(i+1)),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*(i+1))); // canto inferior direito
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
                // segundo triangulo
            fprintf(fd, "glColor3f(1,0,1);\n");
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(j+1))*radius*sin(alfa*i),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*i)); // canto superior esquero
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*j)*radius*sin(alfa*i),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero

            //metade inferior
                // primeiro trinagulo
            fprintf(fd, "glColor3f(1,1,0);\n");
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*j)*radius*sin(alfa*i),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*j)*radius*sin(alfa*(i+1)),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*(i+1))); // canto inferior direito
                // segundo triangulo
            fprintf(fd, "glColor3f(0,1,1);\n");
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*j)*radius*sin(alfa*i),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            fprintf(fd, "glVertex3f(%f,%f,%f);\n",cos(beta*(j+1))*radius*sin(alfa*i),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*i)); // canto superior esquero

                
            }
        }
    }
    fclose(fd);
}

void print_plane(float x,char* file_name){
    FILE *fd = fopen(file_name,"w");
    if (fd){
        float l = x/2;
        // primeiro triangulo
        fprintf(fd,"%f,0,%f\n",-l,l); 
        fprintf(fd,"%f,0,%f\n",l,l);
        fprintf(fd,"%f,0,%f\n",l,-l);

        // segundo triangulo
        fprintf(fd,"%f,0,%f\n",l,-l);
        fprintf(fd,"%f,0,%f\n",-l,-l);
        fprintf(fd,"%f,0,%f\n",-l,l);
    }
    fclose(fd);
}


void print_box(float x,float y,float z,float divisions,char* file_name){
    FILE *fd = fopen(file_name,"w");

    float div_x = x/divisions;
    float div_y = y/divisions;
    float div_z = z/divisions;
    float x_div = x/2;
    float y_div = y/2;
    float z_div = z/2;

    if (fd){
        
        //XY +Z axis

        int i, j = 0;
        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(0,1,0);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*j),z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*j),z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),z_div);

                //segundo triangulo
                
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*j),z_div);
            }
        }

        //XY -Z axis
        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(0,1,1);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);

                //segundo triangulo
                
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*j),-z_div);
            }
        }

        //XZ +Y axis
        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(1,1,0);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*j);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));

                //segundo triangulo
                
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
            }
        }

        //XZ -Y axis
        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(0.5,0,0.5);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),-y_div,z_div-div_z*j);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),-y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*(j+1));

                //segundo triangulo
                
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*j);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),-y_div,z_div-div_z*j);
            }
        }


        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(0.2,0.2,0.6);\n");
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));

                //segundo triangulo
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
            }
        }

        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(1,1,1);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));

                //segundo triangulo
            
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*i);
            }
        }


        


    }
    fclose(fd);
}

void print_cone(float radius,float height,float slices,float stacks,char* file_name){
    FILE *fd = fopen(file_name,"w");
    if (fd){
          // 1 slice vai dividir o cone em 8 partes 
        float alfa = 2*M_PI / (slices*4);
        float rR = 1/stacks ; // multipicador do raio

        float d = 1; // d é o tamanho de cada divisao
        if(stacks > 1) d = height/stacks;  // caso 1 stack a divisao por 0

        //fade
        float fade = 1;


        ///////////////////////////////////////  Criação da base ////////////////////////////////////////////

        // desenhar a base 
        // iniciar no eixo do Z
        for(int i=0;i<(slices*4);i++){

            fprintf(fd, "glColor3f(%f,0,0);\n" ,fade);
            fprintf(fd, "glVertex3f(%f,0,%f);\n", sin(alfa*i)*radius, cos(alfa*i)*radius);
            //ponto fixo do centro
            fprintf(fd, "glVertex3f(0,0,0);\n");
            fprintf(fd, "glVertex3f(%f,0,%f);\n", sin(alfa*(i+1))*radius, cos(alfa*(i+1))*radius);

            fprintf(fd, "glColor3f(%f,0,0);\n", fade);
            fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*i)*radius*rR, height - d, cos(alfa*i)*radius*rR);
            fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*(i+1))*radius*rR, height - d , cos(alfa*(i+1))*radius*rR);
            //ponto fixo do centro
            fprintf(fd, "glVertex3f(0,%f,0);\n",height);
            
        }

        /////////////////////////////////////// Criação do corpo //////////////////////////////////////////////
        fade = 1;
        for(int j=0; j < stacks-1; j++){ // camadas 
            //começamos da base e vamos subindo i é a camada inferior
            for(int i=0;i<(slices*4);i++) { // circunferencia
                fprintf(fd, "glColor3f(%f,0,0);\n", fade);
                                                                          // 1-(rR*0) = 1 primeira camada
                fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*i)*radius*(1-(rR*j))  ,j*d, cos(alfa*i)*radius*(1-(rR*j))); // canto inferior esquerdo
                fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*(i+1))*radius*(1-(rR*j)),j*d, cos(alfa*(i+1))*radius*(1-(rR*j))); // canto inferior direito
                fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*(i+1))*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*(i+1))*radius*(1-(rR*(j+1)))); // canto superior direito

                fprintf(fd, "glColor3f(0,0,%f);\n", fade);
                fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*(i+1))*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*(i+1))*radius*(1-(rR*(j+1)))); // canto superior direito
                fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*i)*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*i)*radius*(1-(rR*(j+1)))); // canto superior esquerdo
                fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*i)*radius*(1-(rR*j)),j*d, cos(alfa*i)*radius*(1-(rR*j))); // canto inferior esquerdo
            }
        }

        //////////////////////////////////////// Criação da ponta //////////////////////////////////////////////
        /*
        fade = 1;
        for(int i=0;i<(slices*8);i++){
            fprintf(fd, "glColor3f(%f,0,0);\n", fade);

            fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*i)*radius*rR, height - d, cos(alfa*i)*radius*rR);
            fprintf(fd, "glVertex3f(%f,%f,%f);\n", sin(alfa*(i+1))*radius*rR, height - d , cos(alfa*(i+1))*radius*rR);
            //ponto fixo do centro
            fprintf(fd, "glVertex3f(0,%f,0);\n",height);
        }*/


    }
    fclose(fd);

}



