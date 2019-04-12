#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>    
using namespace std;

typedef struct bmpFileHeader{
    unsigned int size;    
    unsigned short resv1; 
    unsigned short resv2; 
    unsigned int offset;  
} bmpFileHeader;

typedef struct bmpInfoHeader
{
    unsigned int tam_cabezera;    
    unsigned int ancho;           
    unsigned int alto;            
    unsigned short num_planos;    
    unsigned short bits_ppixel;   
    unsigned int compresion;      
    unsigned int tam_img;         
    unsigned int res_x;           
    unsigned int res_y;           
    unsigned int tam_tablaColor;  
    unsigned int cont_col_Import; 
} bmpInfoHeader;

unsigned char *readBMP(string filename, bmpFileHeader *fheader, bmpInfoHeader *bInfoHeader)
{
    FILE *f;
    // bmpFileHeader header;   
    unsigned char *imgdata; 
    unsigned short type;    

    f = fopen(filename.c_str(), "rb");
    if (!f)
        return NULL; 

    
    fread(&type, sizeof(unsigned short), 1, f);
    if (type != 0x4D42) /* Comprobamos el formato BM*/
    {
        fclose(f);
        return NULL;
    }

    fread(fheader, sizeof(bmpFileHeader), 1, f);
    // fheader = &header;
    fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);
    imgdata = new unsigned char[bInfoHeader->tam_img];
    fseek(f, fheader->offset, SEEK_SET);
    fread(imgdata, bInfoHeader->tam_img, 1, f);
    fclose(f);

    return imgdata;
}

unsigned char *sumar(bmpInfoHeader info, unsigned char *img, int cuanto)
{
    int x, y;
    unsigned char r, g, b;
    unsigned char *edit;
    edit = new unsigned char[info.tam_img];
    
    /* Dibujamos la imagen */
    for (y = info.alto; y > 0; y--)
    {
        for (x = 0; x < info.ancho; x++)
        {
            b = (img[3 * (x + y * info.ancho)]);
            g = (img[3 * (x + y * info.ancho) + 1]);
            r = (img[3 * (x + y * info.ancho) + 2]);
            // cout << "B: " << int(b) << " G: " << int(g) << " R: " << int(r) << endl;
            edit[3 * (x + y * info.ancho)] = b + cuanto;
            edit[3 * (x + y * info.ancho) + 1] = g + cuanto;
            edit[3 * (x + y * info.ancho) + 2] = r + cuanto;

        }
    }
    return edit;
}
unsigned char *sumarg(bmpInfoHeader info, unsigned char *img, int cuanto)
{
    int x, y;
    unsigned char gris;
   // unsigned char r, g, b;
    unsigned char *edit;
    edit = new unsigned char[info.tam_img];
    
    /* Dibujamos la imagen */
    for (y = info.alto; y > 0; y--)
    {
        for (x = 0; x < info.ancho; x++)
        {
            gris = (img[(x + y * info.ancho)]);
            //g = (img[3 * (x + y * info.ancho) + 1]);
            //r = (img[3 * (x + y * info.ancho) + 2]);
            // cout << "B: " << int(b) << " G: " << int(g) << " R: " << int(r) << endl;
            edit[(x + y * info.ancho)] = gris + cuanto;
            //edit[3 * (x + y * info.ancho) + 1] = g + cuanto;
            //edit[3 * (x + y * info.ancho) + 2] = r + cuanto;

        }
    }
    return edit;
}


void DisplayInfo(bmpInfoHeader *info)
{
    cout << "Tamaño de la cabecera: " << info->tam_cabezera << endl;
    cout << "Anchura: " << info->ancho << endl;
    cout << "Altura: " << info->alto << endl;
    cout << "Planos (1): " << info->num_planos << endl;
    cout << "Bits por pixel: " << info->bits_ppixel << endl;
    cout << "Compresión: " << info->compresion << endl;
    cout << "Tamaño de datos de imagen: " << info->tam_img << endl;
    cout << "Resolucón horizontal: " << info->res_x << endl;
    cout << "Resolucón vertical: " << info->res_y << endl;
    cout << "Colores en paleta: " << info->tam_tablaColor << endl;
    cout << "Colores importantes: " << info->cont_col_Import << endl;
}

void saveImg(string nom_saveBMP, bmpInfoHeader infoHead, bmpFileHeader fileHead, unsigned char *imgdata)
{
    
    unsigned short type = 0x4D42;                     
    FILE *outfile = fopen(nom_saveBMP.c_str(), "wb"); // wb -> w: writable b: binary, open as writable and binary
    if (outfile == NULL)
    {
        cout << "Archivo no encontrado ";
    }
    
    fwrite(&type, sizeof(unsigned short), 1, outfile);
    fwrite(&fileHead, sizeof(bmpFileHeader), 1, outfile);
    fwrite(&infoHead, sizeof(bmpInfoHeader), 1, outfile);
    fwrite(imgdata, infoHead.tam_img, 1, outfile);
    
    fclose(outfile);
}
