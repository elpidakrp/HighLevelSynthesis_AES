#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <string>
#include <bitset>

#include "ac_int.h"

ac_int<8,false> sbox[16][16] ={
 {0x63 ,0x7c ,0x77 ,0x7b ,0xf2 ,0x6b ,0x6f ,0xc5 ,0x30 ,0x01 ,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76},
 {0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59 ,0x47 ,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0},
 {0xb7 ,0xfd ,0x93 ,0x26 ,0x36 ,0x3f ,0xf7 ,0xcc ,0x34 ,0xa5 ,0xe5 ,0xf1 ,0x71 ,0xd8 ,0x31 ,0x15},
 {0x04 ,0xc7 ,0x23 ,0xc3 ,0x18 ,0x96 ,0x05 ,0x9a ,0x07 ,0x12 ,0x80 ,0xe2 ,0xeb ,0x27 ,0xb2 ,0x75},
 {0x09 ,0x83 ,0x2c ,0x1a ,0x1b ,0x6e ,0x5a ,0xa0 ,0x52 ,0x3b ,0xd6 ,0xb3 ,0x29 ,0xe3 ,0x2f ,0x84},
 {0x53 ,0xd1 ,0x00 ,0xed ,0x20 ,0xfc ,0xb1 ,0x5b ,0x6a ,0xcb ,0xbe ,0x39 ,0x4a ,0x4c ,0x58 ,0xcf},
 {0xd0 ,0xef ,0xaa ,0xfb ,0x43 ,0x4d ,0x33 ,0x85 ,0x45 ,0xf9 ,0x02 ,0x7f ,0x50 ,0x3c ,0x9f ,0xa8},
 {0x51 ,0xa3 ,0x40 ,0x8f ,0x92 ,0x9d ,0x38 ,0xf5 ,0xbc ,0xb6 ,0xda ,0x21 ,0x10 ,0xff ,0xf3 ,0xd2},
 {0xcd ,0x0c ,0x13 ,0xec ,0x5f ,0x97 ,0x44 ,0x17 ,0xc4 ,0xa7 ,0x7e ,0x3d ,0x64 ,0x5d ,0x19 ,0x73},
 {0x60 ,0x81 ,0x4f ,0xdc ,0x22 ,0x2a ,0x90 ,0x88 ,0x46 ,0xee ,0xb8 ,0x14 ,0xde ,0x5e ,0x0b ,0xdb},
 {0xe0 ,0x32 ,0x3a ,0x0a ,0x49 ,0x06 ,0x24 ,0x5c ,0xc2 ,0xd3 ,0xac ,0x62 ,0x91 ,0x95 ,0xe4 ,0x79},
 {0xe7 ,0xc8 ,0x37 ,0x6d ,0x8d ,0xd5 ,0x4e ,0xa9 ,0x6c ,0x56 ,0xf4 ,0xea ,0x65 ,0x7a ,0xae ,0x08},
 {0xba ,0x78 ,0x25 ,0x2e ,0x1c ,0xa6 ,0xb4 ,0xc6 ,0xe8 ,0xdd ,0x74 ,0x1f ,0x4b ,0xbd ,0x8b ,0x8a},
 {0x70 ,0x3e ,0xb5 ,0x66 ,0x48 ,0x03 ,0xf6 ,0x0e ,0x61 ,0x35 ,0x57 ,0xb9 ,0x86 ,0xc1 ,0x1d ,0x9e},
 {0xe1 ,0xf8 ,0x98 ,0x11 ,0x69 ,0xd9 ,0x8e ,0x94 ,0x9b ,0x1e ,0x87 ,0xe9 ,0xce ,0x55 ,0x28 ,0xdf},
 {0x8c ,0xa1 ,0x89 ,0x0d ,0xbf ,0xe6 ,0x42 ,0x68 ,0x41 ,0x99 ,0x2d ,0x0f ,0xb0 ,0x54 ,0xbb ,0x16}
};

int Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};


void XorRoundKey(ac_int<32,false> state_array[4],ac_int<128,false> &Rkey){
    short iter = 0;
    for(int j = 0; j < 4; j++){
        for(int i = 0; i < 4; i++){
            state_array[i].set_slc(j*8,state_array[i].slc<8>(j*8)^Rkey.slc<8>(8*iter));
            iter++;
        }
    }
}

void key_generation(ac_int<128,false> &Rkey, short num_ofkey){
    ac_int<4,false> row;
    ac_int<4,false> col;
    ac_int<8,false> tmp[4];
    ac_int<32,false> sub_word;
    ac_int<32,false> W[4] = {Rkey.slc<32>(0),Rkey.slc<32>(32), Rkey.slc<32>(64), Rkey.slc<32>(96)};
    
    tmp[0] = W[3].slc<8>(8);
    tmp[1] = W[3].slc<8>(16);
    tmp[2] = W[3].slc<8>(24);
    tmp[3] = W[3].slc<8>(0);

    for(int i = 0; i < 4; i++){
        row    = tmp[i].slc<4>(4);
        col    = tmp[i].slc<4>(0);
        tmp[i] = sbox[row][col];
    }
    sub_word.set_slc(0,tmp[0]);
    sub_word.set_slc(8,tmp[1]);
    sub_word.set_slc(16,tmp[2]);
    sub_word.set_slc(24,tmp[3]);
    
    sub_word=sub_word^Rcon[num_ofkey];

    W[0]   = W[0]^sub_word;
    W[1]   = W[1]^W[0];
    W[2]   = W[2]^W[1];
    W[3]   = W[3]^W[2];

    Rkey.set_slc(0,W[0]);
    Rkey.set_slc(32,W[1]);
    Rkey.set_slc(64,W[2]);
    Rkey.set_slc(96,W[3]);

    for(int i = 0; i <16 ;i++){
        tmp[0]=Rkey.slc<8>(i*8);
    }
}

void SubBytes(ac_int<32,false> state_array[4]){
    ac_int<4,false> row;
    ac_int<4,false> col;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            row = state_array[i].slc<4>(j*8+4);
            col = state_array[i].slc<4>(j*8);
            state_array[i].set_slc(j*8,sbox[row][col]);
        }
    }
}

void ShiftRows(ac_int<32,false> state_array[4]){
    // 2nd row left shift 1 step
    state_array[1] = (state_array[1] >> 8) | (state_array[1] << (32 - 8));

    // 3rd row left shift 2 steps
    state_array[2] = (state_array[2] >> 16) | (state_array[2] << (32 - 16));
    
    //4th row left shift 3 steps
    state_array[3] = (state_array[3] >> 24) | (state_array[3] << (32 - 24));
}

void MixColumns(ac_int<32,false> state_array[4]){
    ac_int<8,false> mix_temp[4];
    ac_int<8,false> b[4];
    int h;
    for(int j = 0; j < 4; j++){
        for(int g = 0; g < 4;g++){
            h    = state_array[g].slc<8>(j*8)&0x80;
            b[g] = state_array[g].slc<8>(j*8) << 1;
            if(h == 0x80){
                b[g]^=0x1b;
            }

        }

        mix_temp[0] = b[0] ^ (b[1]^state_array[1].slc<8>(j*8)) ^ state_array[2].slc<8>(j*8) ^ state_array[3].slc<8>(j*8);
        mix_temp[1] = state_array[0].slc<8>(j*8) ^ b[1] ^ (b[2]^ state_array[2].slc<8>(j*8)) ^ state_array[3].slc<8>(j*8);
        mix_temp[2] = state_array[0].slc<8>(j*8) ^ state_array[1].slc<8>(j*8) ^ b[2]^ (b[3]^ state_array[3].slc<8>(j*8));
        mix_temp[3] = (b[0] ^ state_array[0].slc<8>(j*8)) ^ state_array[1].slc<8>(j*8) ^ state_array[2].slc<8>(j*8) ^ b[3] ;

        state_array[0].set_slc(j*8,mix_temp[0]);
        state_array[1].set_slc(j*8,mix_temp[1]);
        state_array[2].set_slc(j*8,mix_temp[2]);
        state_array[3].set_slc(j*8,mix_temp[3]);
    }
}



int main() {
    FILE *f;
    ac_int<8,false> num[80000];
    int i=0;
    int rv;
    int num_values;
    int size = 128;

    ac_int<128,false> Rkey ;
    ac_int<128,false> block ;
    ac_int<128,false> Output ;
    ac_int<128,false> Computed_output ;
    int iter=0;
    int part = 0;
    int count[8000] = {0};
    std::bitset<8> character;
    ac_int<8,false> plain_text[16];
    ac_int<8,false> rkey[16];
    ac_int<8,false> out[16];
    ac_int<32,false> state_array[4];
    int error_iter[8000];

    f=fopen("ECBVarTxt128_128.txt","r"); //CHANGE SIZE LIMIT @LINE 210
    if (f==NULL){
        printf("file doesnt exist?!\n");
        return 1;
    }

    while (i < 80000) {
        rv = fscanf(f, "%2x", &num[i]);

        if (rv != 1)
            break;

        i++;
    }
    fclose(f);
    num_values = i;

    printf("Successfully read %d values:\n", num_values);
    // for (i = 0; i < 40; i++)
    // {
    //     printf("\t%x\n", num[i]);
    // }

    for (int i=0; i<size; i++){ //iterations
        part = 0;
        count[i+1] = count[i]+1;
        // std::cout<<"count: "<<count[i]<<std::endl;

        // std::cout<<"Rkey: ";
        for(int j=0; j<16; j++){
            rkey[j] = count[i]<100?num[i*49+1+j]:num[i*49+1+j+(i-99)];
            // std::cout<<std::hex<<rkey[j]<<std::endl;
        }
        
        for(int i = 0; i < 16; i++){
            Rkey.set_slc(i*8,rkey[i]);
        }
        // std::cout<<std::hex<<Rkey<<std::endl;

        // std::cout<<"State Array: "<<std::endl;
        for(int j=0; j<16; j++){
            plain_text[j] = count[i]<100?num[i*49+17+j]:num[i*49+17+j+(i-99)];
        }
        for(int j = 0; j < 4; j++){
            for(int i = 0; i < 4; i++){
                state_array[i].set_slc(j*8,plain_text[part]);
                // std::cout<<plain_text[part];
                part++;
            }
            // std::cout<<std::endl;
        }

        // std::cout<<"Output:          ";
        for(int j=0; j<16; j++){
            out[j] = count[i]<100?num[i*49+33+j]:num[i*49+33+j+(i-99)];
        }
        for(int i = 0; i < 16; i++){
            Output.set_slc(i*8,out[15-i]);
        }
        // std::cout<<std::hex<<Output<<std::endl;

        XorRoundKey(state_array, Rkey);
        for(int i = 0; i < 9; i++){
            // std::cout<<"Round "<<i+1<<std::endl;
            SubBytes(state_array);
            ShiftRows(state_array);
            MixColumns(state_array);
            key_generation(Rkey,i);
            XorRoundKey(state_array, Rkey);
        }
        // std::cout<<"Round 10"<<std::endl;
        SubBytes(state_array);
        ShiftRows(state_array);
        key_generation(Rkey,9);
        XorRoundKey(state_array, Rkey);

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                Computed_output.set_slc(120-(i*32+j*8),state_array[j].slc<8>(i*8));
            }
        }
        // std::cout<<"Computed output: ";
        // std::cout<<std::hex<<Computed_output<<std::endl;

        if(Output!=Computed_output){
            error_iter[count[i]] = count[i]+1;
            iter++;
        }else
        {
            error_iter[count[i]] = 0;
        }
        
    }

    std::cout<<"Errors: "<<iter<<std::endl;
    std::cout<<"Error: "<<double(iter*100/size)<<"%"<<std::endl;
    // for(int i=0; i<size; i++){
    //     std::cout<<error_iter[i]<<std::endl;
    // }

    return 0;
}