#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

#include "ac_int.h"
#include "ac_channel.h"
#include "mc_scverify.h"


#define TEST 5

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

ac_int<8,false> invsbox[16][16] =
{
	{0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB}, 
    {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB}, 
    {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E}, 
    {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25}, 
    {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92}, 
    {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84}, 
    {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06}, 
    {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B}, 
    {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73}, 
    {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E}, 
    {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B}, 
    {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4}, 
    {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F}, 
    {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF}, 
    {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61}, 
    {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
};

ac_int<8,false> Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};


void print_channel(ac_channel<int> &channel_in){
    ac_int<8,false>temp;
    ac_int<32,false> read_channel;
    if(channel_in.available(1)){
        for(int times = 0 ; times < 4; times++){
            read_channel = channel_in.read();
            for(int i = 0; i < 4; i ++){
                temp = read_channel.slc<8>(8*i);
                std::cout<<" "<<std::hex<<temp<<" ";
            }
        }
    }

}



template<int id> void InvXorRoundKey(ac_channel<int> &state_array,ac_channel<int> &Rkey,ac_channel<int> &invxorroundkey){
    ac_int<32,false>state, key;
    if(state_array.available(4) && Rkey.available(4)){
        XOR_BYTE:for(int byte = 0; byte < 4; byte++){
            state = state_array.read();
            key = Rkey.read();
            state = state ^ key;
            invxorroundkey.write(state);
        }
    }  
}


ac_int<128,false> key_generation(ac_int<128,false> Rkey, short num_ofkey){
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
    
    return Rkey;
}

template<int id> void InvSubBytes(ac_channel<int> &state_array, ac_channel<int> &ready){
    ac_int<8,false> state[4][4];
    ac_int<32,false> col;
    ac_int<32,false> tmp;

    if(state_array.available(4)){   
        SUB_BYTES_I:for(int i = 0; i < 4; i++){
            tmp=state_array.read();
            SUB_BYTES_J:for(int j = 0; j < 4; j++){
                state[i][j] = invsbox[tmp.slc<8>(j*8).slc<4>(4)][tmp.slc<8>(j*8).slc<4>(0)];
                col.set_slc(8*j,state[i][j]);
            }
            ready.write(col);
        }
    }   
    

}

template<int id> void InvShiftRows(ac_channel<int> &input_col, ac_channel<int> &shiftrows_out){
    ac_int<8,false> temp1,temp2,temp3;
    ac_int<8,false> state_array[4][4];
    ac_int<32,false> temp_col;
    if(input_col.available(4)){
        SHIFT_ROWS_IN_I:for(int i = 0; i < 4; i++){
            temp_col = input_col.read();
            SHIFT_ROWS_IN_J:for (int j = 0; j < 4; j++){
                state_array[j][i] = temp_col.slc<8>(j*8);
            }
        }
      
        // 2nd row right shift 1 step
        temp1 = state_array[1][3];
        state_array[1][3]=state_array[1][2];
        state_array[1][2]=state_array[1][1];
        state_array[1][1]=state_array[1][0];
        state_array[1][0]=temp1;


        // 3rd row right shift 2 steps
        temp1 = state_array[2][2];
        temp2 = state_array[2][3];
        state_array[2][3]=state_array[2][1];
        state_array[2][2]=state_array[2][0];
        state_array[2][1]=temp2;
        state_array[2][0]=temp1;
        
        //4th row right shift 3 steps
        temp1 = state_array[3][1];
        temp2 = state_array[3][2];
        temp3 = state_array[3][3];
        state_array[3][3]=state_array[3][0];
        state_array[3][2]=temp3;
        state_array[3][1]=temp2;
        state_array[3][0]=temp1;

        SHIFT_ROWS_OUT_I:for(int i = 0; i < 4; i++){
            SHIFT_ROWS_OUT_J:for(int j = 0; j < 4; j++){
                temp_col.set_slc(j*8,state_array[j][i]);
            }
        shiftrows_out.write(temp_col);
        } 
    }
}

ac_int<8,false> galoiMUL( ac_int<8,false> a,  ac_int<8,false> b){
    ac_int<8,false> res = 0;
    for (int i=0; i<8; i++){
        if(b>=0){
            if(b&1){
                res^=a;
            }
            if(a&0x80){
                a=(a<<1)^0x11B;
            }
            else{
                a<<=1;
            }
            b>>=1;
        }
    }
    return res;
}

template<int id> void InvMixColumns( ac_channel<int> &input_col ,ac_channel<int> &invmixcolumns_out){
    ac_int<8,false> mix_temp[4];
    ac_int<32,false> tmp;
    ac_int<8,false> b[4];
    ac_int<32,false> temp_col;
    ac_int<8,false> state_array[4][4];
    int h;

    if(input_col.available(1)){
        MIX_COL_IN_I:for(int i = 0; i < 4; i++){
            temp_col = input_col.read();
            MIX_COL_IN_J:for(int j = 0; j < 4; j++){
                state_array[j][i] = temp_col.slc<8>(8*j);
            }
        }

        
        for(int j = 0; j < 4; j++){
            mix_temp[0] = galoiMUL(state_array[0][j],0x0e)^galoiMUL(state_array[1][j],0x0b)^galoiMUL(state_array[2][j],0x0d)^galoiMUL(state_array[3][j],0x09);
            mix_temp[1] = galoiMUL(state_array[0][j],0x09)^galoiMUL(state_array[1][j],0x0e)^galoiMUL(state_array[2][j],0x0b)^galoiMUL(state_array[3][j],0x0d);
            mix_temp[2] = galoiMUL(state_array[0][j],0x0d)^galoiMUL(state_array[1][j],0x09)^galoiMUL(state_array[2][j],0x0e)^galoiMUL(state_array[3][j],0x0b);
            mix_temp[3] = galoiMUL(state_array[0][j],0x0b)^galoiMUL(state_array[1][j],0x0d)^galoiMUL(state_array[2][j],0x09)^galoiMUL(state_array[3][j],0x0e);

            tmp.set_slc(0,mix_temp[0]);
            tmp.set_slc(8,mix_temp[1]);
            tmp.set_slc(16,mix_temp[2]);
            tmp.set_slc(24,mix_temp[3]);
            invmixcolumns_out.write(tmp);
        }        
    }
}

template <int id, int rnd> void round (ac_channel<int> &input, ac_channel<int> &round_out, ac_channel<int> &key ){
    static ac_channel<int> subbyte_out, xorround_out, shiftrows_out, mixcolumns_out, data_in_key, sub_ready, shift_ready, out;
   
    InvShiftRows<id>(input, shiftrows_out);
    InvSubBytes<id>(shiftrows_out,subbyte_out); 
    InvXorRoundKey<id+1>(subbyte_out,key,xorround_out); 
    InvMixColumns<id>(xorround_out,round_out);
    
}
void split_keys(ac_channel<int> &input_key,ac_channel<int> output_key[11]){
    ac_int<32,false> word_key;
    if(input_key.available(44)){
        for(int key = 0; key < 11; key++) {
            for(int i = 0; i < 4; i++){
                word_key = input_key.read();
                output_key[10-key].write(word_key);
            }
        }
    }
}


#pragma hls_design top
void CCS_BLOCK(decrypt_hier_block)(ac_int<8,false> state_array[4][4],ac_int<128,false> Rkey[11],ac_channel<int> &comp_out){
    static ac_channel<int>new_key[11],output_key[11],data_in_state,data_in_key, mixcolumns_out,subbyte_out, shiftrows_out,  res, invxorroundkey_out, round1_out,round2_out,round3_out,round4_out,round5_out,round6_out,round7_out,round8_out,round9_out;
    ac_int<32,false> data_in;
    
    DECR_I:for(int i = 0; i < 4; i++){
        DECR_J:for(int j = 0; j < 4; j++){
            data_in.set_slc(8*j,state_array[j][i]);
        }
        data_in_state.write(data_in);
    }

    IN_KEY_I:for(int i = 10; i>=0; i--){
        IN_KEY_WORD:for(int word = 0; word < 4; word++){
            data_in_key.write(Rkey[i].slc<32>(word*32));
        }
    }
    

    
    

    split_keys(data_in_key,output_key);
    InvXorRoundKey<1>(data_in_state,output_key[10],invxorroundkey_out);
    round<1,1>(invxorroundkey_out,round1_out,output_key[9]);
    round<2,2>(round1_out,round2_out,output_key[8]); 
    round<3,3>(round2_out,round3_out,output_key[7]); 
    round<4,4>(round3_out,round4_out,output_key[6]); 
    round<5,5>(round4_out,round5_out,output_key[5]); 
    round<6,6>(round5_out,round6_out,output_key[4]); 
    round<7,7>(round6_out,round7_out,output_key[3]); 
    round<8,8>(round7_out,round8_out,output_key[2]); 
    round<9,9>(round8_out,round9_out,output_key[1]);
    

    InvShiftRows<10>(round9_out, shiftrows_out);
    InvSubBytes<10>(shiftrows_out,subbyte_out); 
    
    InvXorRoundKey<11>(subbyte_out,output_key[0],comp_out);
    // print_channel(comp_out);

}


CCS_MAIN (int argc, char* argv[]){
    ac_int<128,false> Rkey[11] ;
    ac_int<128,false> block ;
    int iter=0;
    int part = 0;
    ac_int<8,false> cypher_text[TEST][16];//={0x69,0xc4,0xe0,0xd8,0x6a,0x7b,0x04,0x30,0xd8,0xcd,0xb7,0x80,0x70,0xb4,0xc5,0x5a};
    ac_int<8,false> rkey[TEST][16];//={0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    ac_int<8,false> state_array[4][4];

    ac_channel<int> ready,ready_out;
    ac_channel<int> col,invxorroundkey_out,data_in_state,data_in_key,comp_out;
    ac_int<32,false> data_in,out;

    
    // Generating input data
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 16;j++){
            cypher_text[i][j] = rand()%255;
        }
    }
    
    
    // Generating key data
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 16;j++){
            rkey[i][j] = rand()%255;
        }
    }
    
    for(int test = 0; test < TEST; test++){
        for(int i = 0; i < 16; i++){
            Rkey[0].set_slc(i*8,rkey[test][i]);
        }

        for(int key = 1; key < 11; key++){
            Rkey[key]=key_generation(Rkey[key-1],key-1);
        }
        
        // Create State Array
        for(int j = 0; j < 4; j++){
            for(int i = 0; i < 4; i++){
            state_array[i][j]=cypher_text[test][part];
            part++;
            }
        }
    

        decrypt_hier_block(state_array,Rkey,comp_out);
    }
    

    return 0;
}