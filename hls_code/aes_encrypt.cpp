#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

#include "ac_int.h"
//#include "mc_scverify.h"

#define TEST 1

int sbox[16][16] ={
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

void XorRoundKey(ac_int<8,false> state_array[4][4],ac_int<128,false> &Rkey){
    short iter = 0;
    XOR_ROUND_KEY_j:for(int j = 0; j < 4; j++){
        XOR_ROUND_KEY_i:for(int i = 0; i < 4; i++){
            state_array[i][j] = state_array[i][j]^Rkey.slc<8>(8*iter);
            iter++;
        }
    }
}

void key_generation(ac_int<128,false> &Rkey, short num_ofkey){
    //ac_int<4,false> row;
    //ac_int<4,false> col;
    ac_int<8,false> tmp;
    ac_int<32,false> sub_word;
    ac_int<32,false> Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};
    ac_int<32,false> W[4] = {Rkey.slc<32>(0),Rkey.slc<32>(32), Rkey.slc<32>(64), Rkey.slc<32>(96)};
    
    
    // Shifting 
    sub_word.set_slc(0, W[3].slc<8>(8));
    sub_word.set_slc(8, W[3].slc<8>(16));
    sub_word.set_slc(16,W[3].slc<8>(24));
    sub_word.set_slc(24,W[3].slc<8>(0));

    //Substitution
    KEY_GENERATION:for(int i = 0; i < 4; i++){
        tmp = sbox[sub_word.slc<4>(8*i+4)][sub_word.slc<4>(8*i)];
        sub_word.set_slc(i*8,tmp);
    }
    
    sub_word=sub_word^Rcon[num_ofkey];
    
    // XORS
    W[0]   = W[0]^sub_word;
    W[1]   = W[1]^W[0];
    W[2]   = W[2]^W[1];
    W[3]   = W[3]^W[2];

    Rkey.set_slc(0,W[0]);
    Rkey.set_slc(32,W[1]);
    Rkey.set_slc(64,W[2]);
    Rkey.set_slc(96,W[3]);
}

void SubBytes(ac_int<8,false> state_array[4][4]){
    SUB_BYTES_i:for(int i = 0; i < 4; i++){
        SUB_BYTES_j:for(int j = 0; j < 4; j++){

            state_array[i][j] = sbox[state_array[i][j].slc<4>(4)][state_array[i][j].slc<4>(0)];
        }
    } 
}

void ShiftRows(ac_int<8,false> state_array[4][4]){
    ac_int<8,false> temp1,temp2,temp3;

    // 2nd row left shift 1 step
    temp1 = state_array[1][0];
    state_array[1][0]=state_array[1][1];
    state_array[1][1]=state_array[1][2];
    state_array[1][2]=state_array[1][3];
    state_array[1][3]=temp1;

    // 3rd row left shift 2 steps
    temp1 = state_array[2][0];
    temp2 = state_array[2][1];
    state_array[2][0]=state_array[2][2];
    state_array[2][1]=state_array[2][3];
    state_array[2][2]=temp1;
    state_array[2][3]=temp2;
    
    //4th row left shift 3 steps
    temp1 = state_array[3][0];
    temp2 = state_array[3][1];
    temp3 = state_array[3][2];
    state_array[3][0]=state_array[3][3];
    state_array[3][1]=temp1;
    state_array[3][2]=temp2;
    state_array[3][3]=temp3;

}

void MixColumns(ac_int<8,false> state_array[4][4]){
    ac_int<8,false> b[4];
    ac_int<8,false> c[4];
    short h;
    MIX_COL_j:for(int j = 0; j < 4; j++){
        MIX_COL_g:for(int g = 0; g < 4;g++){
            // Save it iternaly
            c[g]=state_array[g][j];
            // Check for high MSB
            h = c[g]&0x80;
            // MUL by 2
            b[g]=c[g] << 1;
            // Reduce if necessary
            if(h==0x80){
                b[g]^=0x1b;
            }
        }

        // MIX
        state_array[0][j] = b[0] ^ (b[1]^c[1]) ^ c[2] ^ c[3];
        state_array[1][j] = c[0] ^ b[1] ^ (b[2]^ c[2]) ^ c[3];
        state_array[2][j] = c[0] ^ c[1] ^ b[2]^ (b[3]^ c[3]);
        state_array[3][j] = (b[0] ^ c[0]) ^ c[1] ^ c[2] ^ b[3] ;


    }

}
#pragma hls_design top
void /*CCS_BLOCK*/encrypt(ac_int<8,false> state_array[4][4],ac_int<128,false> &Rkey){
    XorRoundKey(state_array, Rkey);
    ENCRYPT:for(int i = 0; i < 9; i++){
        SubBytes(state_array);
        ShiftRows(state_array);
        MixColumns(state_array);
        key_generation(Rkey,i);
        XorRoundKey(state_array, Rkey);
    }
    SubBytes(state_array);
    ShiftRows(state_array);
    key_generation(Rkey,9);
    XorRoundKey(state_array, Rkey);
}

/*CCS_MAIN*/int main(int argc, char* argv[]){
    srand(time(NULL));
    ac_int<128,false> Rkey ;
    int part = 0;
    ac_int<8,false> plain_text[TEST][16]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
    ac_int<8,false> rkey[TEST][16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    ac_int<8,false> state_array[4][4];

    for(int test = 0; test < TEST; test++){ 
        // Create State Array
        part = 0;
        for(int i = 0; i < 16; i++){
        Rkey.set_slc(i*8,rkey[test][i]);
        }
        for(int j = 0; j < 4; j++){
            for(int i = 0; i < 4; i++){
            state_array[i][j]=plain_text[test][part];    
            part++;
            }
        }
        encrypt(state_array,Rkey);
        std::cout<<"Cypher"<<std::endl;
        //print_2Darray(state_array);
        
    }

    return 0;
}