#include <iostream>
#include <string.h>
#include <cstdio>

using namespace std;

typedef struct punteros {
    int now;        //direccion donde se guardo el ultimo registro y se guarda el siguiente
    int root_prof;  //posicion del primer profesor
    int end_prof;   //posicion del ultimo profesor guardado, para actualizar su siguiente
    int root_alum;  //posicion del primer alumno
    int end_alum;   //posicion del ultimo alumno guardado, para actualizar su siguiente
};

typedef struct siguente {
    int next; //guarda la direccion del siguiente profesor o alumno
};

typedef struct Profesor {
    int id_profesor;// 4 bytes
    char cui[13];//   13 bytes
    char nombre[25];// 25 bytes
    char curso[25];//  25 bytes
    //                 sizeof (67)
};

typedef struct Estudiante {
    int id_estudiante;
    char cui[13];
    char nombre[25];
    char carnet[10];
    //                sizeof (52)
};
FILE *archivo = NULL;


int main() {


    archivo = fopen("HT1.bin", "r");
    if (archivo == NULL)//se verifica si ya existe el archivo
    {
        punteros in_punteros;

        archivo = fopen("HT1.bin", "wb+");
        fseek(archivo, 0, SEEK_SET);
        in_punteros.end_alum = -1;
        in_punteros.end_prof = -1;
        in_punteros.root_alum = -1;
        in_punteros.root_prof = -1;
        in_punteros.now = sizeof(in_punteros);
        fwrite(&in_punteros, sizeof(in_punteros), 1, archivo);
        fclose(archivo);
    }

    //archivo = fopen("HT1.bin","r");
    int opmenu = 0;
    while (opmenu != 4) {
        switch (opmenu) {
            case 0: {
                cout << "   ******** MENU ********" << endl;
                cout << endl;
                cout << "1. REGISTRO DE PROFESOR  " << endl;
                cout << "2. REGISTRO DE ESTUDIANTE" << endl;
                cout << "3. VER REGISTROS         " << endl;
                cout << "4. SALIR                 " << endl;
                cout << "\n INGRESE UNA OPCION: ";
                scanf("%i", &opmenu);
            }
                break;

            case 1: {
                system("clear");
                Profesor profe;
                cout << " ****** REGISTRO DE PROFESOR ******" << endl;
                cout << endl;
                cout << " ID_PROFESOR: ";
                scanf("%i", &profe.id_profesor);
                cout << endl;
                cout << " CUI: ";
                scanf("%s", &profe.cui);
                cout << endl;
                cout << " NOMBRE: ";
                scanf("%s", &profe.nombre);
                cout << endl;
                cout << " CURSO: ";
                scanf("%s", &profe.curso);
                cout << endl;

                //FILE *file;
                archivo = fopen("HT1.bin", "ab+");//ab+ sirve para agregar al final de archivo binario

                punteros indice;
                siguente sig;
                fseek(archivo, 0, SEEK_SET);//muevo al inicio de archivo

                //consigo dir punteros
                fread(&indice, sizeof(indice), 1, archivo);//obtengo mi struct indice

                if (indice.root_prof == -1) { //verifico si raiz profesor esta vacía
                    indice.root_prof = indice.now; // si esta vacia actualizo la direccion
                } else {
                    fseek(archivo, indice.end_prof, SEEK_SET);
                    fread(&sig, sizeof(sig), 1, archivo);
                    sig.next = indice.now;//queda al inicio, aqui esta la direccion donde empieza el profesor
                    fwrite(&sig, sizeof(sig), 1, archivo);//guardo siguiente
                }
                fseek(archivo, indice.now, SEEK_SET);// movimiento de posicion de guardar struct profesor
                sig.next = -1;
                fwrite(&profe, sizeof(profe), 1, archivo);//ALMACENO STRUCT PROFESOR
                fclose(archivo);
                archivo = fopen("HT1.bin", "rb+");//para leer y actualizar rb+
                fseek(archivo, indice.now + sizeof(profe), SEEK_SET);//mover puntero al final de struct profesor + now
                fwrite(&sig, sizeof(sig), 1, archivo);//guardo siguiente -1 ya que no hay siguiente aun es el ultimo
                indice.end_prof = indice.now + sizeof(profe);
                fseek(archivo, 0, SEEK_SET);
                indice.now = sizeof(profe) + sizeof(sig) + indice.now;
                fwrite(&indice, sizeof(indice), 1, archivo);


                /*fseek(archivo, 92, SEEK_SET);
                Profesor pfs;
                fread(&pfs, sizeof(pfs), 1, archivo);
                cout << pfs.nombre << endl;
                cout << pfs.cui << endl;
                cout << pfs.curso << endl;
                cout << pfs.id_profesor << endl;*/

                fclose(archivo);

                cout << "SE HA REGISTRADO UN NUEVO PROFESOR CON EXITO" << endl;
                //}
                opmenu = 0;
            }
                break;
            case 2: {
                system("clear");
                Estudiante estu;
                cout << " ****** REGISTRO DE ALUMNO ******" << endl;
                cout << endl;
                cout << " ID_ESTUDIANTE: ";
                scanf("%i", &estu.id_estudiante);
                cout << endl;
                cout << " CUI: ";
                scanf("%s", &estu.cui);
                cout << endl;
                cout << " NOMBRE: ";
                scanf("%s", &estu.nombre);
                cout << endl;
                cout << " CURSO: ";
                scanf("%s", &estu.carnet);
                cout << endl;

                FILE *file;
                file = fopen("HT1.bin", "ab");
                if (file == NULL) {
                    cout << "ARCHIVO INACCESIBLE";
                } else {
                    fwrite(&estu, sizeof(estu), 1, file);
                    fclose(file);
                    cout << "SE HA REGISTRADO UN NUEVO ALUMNO CON EXITO" << endl;
                }
                opmenu = 0;
            }
                break;
            case 3: {
                system("clear");
                FILE *file;
                Profesor profesreport;
                file = fopen("HT1.bin", "rb");
                if (file == NULL) {
                    cout << "NO SE PUEDE ACCEDER AL ARCHIVO";
                } else {
                    system("clear");
                    fread(&profesreport, sizeof(profesreport), 1, file);
                    cout << "****** REGISTROS ******" << endl;
                    while (!feof(file)) {
                        cout << "****** PROFESOR ******" << endl;
                        cout << "ID: " << profesreport.id_profesor << endl;
                        cout << "CUI: " << profesreport.cui << endl;
                        cout << "NOMBRE: " << profesreport.nombre << endl;
                        cout << "CURSO: " << profesreport.curso << endl;

                        cout << "****** ESTUDIANTE ******" << endl;


                        cout << " - - - - - - - - - - - - - - - - - - - - " << endl;
                        fread(&profesreport, sizeof(profesreport), 1, file);
                    }
                    fclose(file);
                }
                opmenu = 0;
            }
                break;
            default:
                cout << "INGRESE UNA OPCION CORRECTA" << endl;
                opmenu = 0;

        }
    }
    return 0;
}
