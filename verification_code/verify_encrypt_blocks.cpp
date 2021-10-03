#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

#include "ac_int.h"
#include "ac_channel.h"

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

ac_int<8,false> Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};


/*void print_channel(ac_channel<int> &channel_in){
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
*/

template<int id> void XorRoundKey(ac_channel<int> &state_array,ac_channel<int> &Rkey,ac_channel<int> &xorroundkey){
    ac_int<32,false>state, key;
    if(state_array.available(4) && Rkey.available(4)){
        XOR_COL:for(int col = 0; col < 4; col++){
            state = state_array.read();
            key = Rkey.read();
            state = state ^ key;
            xorroundkey.write(state);
        }
    }  
}
template<int id> void split(ac_channel<int> &input,ac_channel<int> &output1,ac_channel<int> &output2){
    ac_int<32,false> key;
    if(input.available(4)){
        SPLIT_I:for(int i = 0; i < 4; i++){
            key = input.read();
            output1.write(key);
            output2.write(key);
        }
    }
}
template<int id> void key_generation(ac_channel<int> &Rkey,ac_channel<int> &new_key){
    ac_int<128,false> key;
    ac_int<32,false> sub_word;
    

    if(Rkey.available(4)){
        KEY_GEN_I:for(int i = 0; i < 4; i++){
            sub_word = Rkey.read();
            key.set_slc(32*i,sub_word);
        }
        ac_int<32,false> W[4] = {key.slc<32>(0),key.slc<32>(32), key.slc<32>(64), key.slc<32>(96)};
        
        
        sub_word.set_slc(0,W[3].slc<8>(8));
        sub_word.set_slc(8,W[3].slc<8>(16));
        sub_word.set_slc(16,W[3].slc<8>(24));
        sub_word.set_slc(24,W[3].slc<8>(0));

        KEY_GENERATION:for(int i = 0; i < 4; i++){
            sub_word.set_slc(i*8,sbox[sub_word.slc<8>(i*8).slc<4>(4)][sub_word.slc<8>(i*8).slc<4>(0)]);
        }

        W[0]   = W[0]^sub_word^Rcon[id];
        new_key.write(W[0]);

        W[1]   = W[1]^W[0];
        new_key.write(W[1]);
        
        W[2]   = W[2]^W[1];
        new_key.write(W[2]);
        
        W[3]   = W[3]^W[2];
        new_key.write(W[3]);
    }
}

template<int id> void SubBytes(ac_channel<int> &state_array, ac_channel<int> &ready){
    ac_int<8,false> state[4][4];
    ac_int<32,false> col;
    ac_int<32,false> tmp;

    if(state_array.available(4)){        
        SUB_COL:for(int count_col = 0; count_col < 4; count_col ++){
            tmp=state_array.read();
            SUB_I:for(int i = 0; i < 4; i++){
                state[i][count_col] = sbox[tmp.slc<8>(i*8).slc<4>(4)][tmp.slc<8>(i*8).slc<4>(0)];
                col.set_slc(8*i,state[i][count_col]);
            }
            ready.write(col);
        }
    }
}

template<int id> void ShiftRows(ac_channel<int> &input_col, ac_channel<int> &shiftrows_out){
    ac_int<8,false> temp1,temp2,temp3;
    ac_int<8,false> state_array[4][4];
    ac_int<32,false> temp_col;
    if(input_col.available(4)){
        SHIFT_IN_I:for(int i = 0; i < 4; i++){
            temp_col = input_col.read();
            SHIFT_IN_J:for (int j = 0; j < 4; j++){
                state_array[j][i] = temp_col.slc<8>(j*8);
            }
        }
      
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


        SHIFT_OUT_I:for(int i = 0; i < 4; i++){
            SHIFT_OUT_j:for(int j = 0; j < 4; j++){
                temp_col.set_slc(j*8,state_array[j][i]);
            }
            shiftrows_out.write(temp_col);
        }
    }
}

template<int id> void MixColumns( ac_channel<int> &input_col ,ac_channel<int> &mixcolumns_out){
    ac_int<32,false> mix_temp;
    ac_int<8,false> b[4];
    ac_int<8,false> c[4];
    ac_int<32,false> temp_col;
    ac_int<8,false> state_array[4][4];
    int h;
    if(input_col.available(1)){
        MIX_IN_I:for(int i = 0; i < 4; i++){
            temp_col = input_col.read();
             MIX_IN_J:for(int j = 0; j < 4; j++){
                state_array[j][i] = temp_col.slc<8>(8*j);
            }
        }

        
        MIX_COL_j:for(int j = 0; j < 4; j++){
            MIX_COL_g:for(int g = 0; g < 4;g++){
                c[g]=state_array[g][j];
                h = c[g]&0x80;
                b[g]=c[g] << 1;
                if(h==0x80){
                    b[g]^=0x1b;
                }

            }

            mix_temp.set_slc(0,b[0] ^ (b[1]^c[1]) ^ c[2] ^ c[3]);

            mix_temp.set_slc(8,c[0] ^ b[1] ^ (b[2]^ c[2]) ^ c[3]);
            mix_temp.set_slc(16,c[0] ^ c[1] ^ b[2]^ (b[3]^ c[3]));
            mix_temp.set_slc(24,(b[0] ^ c[0]) ^ c[1] ^ c[2] ^ b[3]);
            mixcolumns_out.write(mix_temp);
        }

    }

}

template <int id, int rnd> void round (ac_channel<int> &input, ac_channel<int> &round_out, ac_channel<int> &key_in, ac_channel<int> &key_out ){
    static ac_channel<int> key_to_xor,new_key,subbyte_out, shiftrows_out, mixcolumns_out, data_in_key;
    
    SubBytes<id>(input,subbyte_out); 
    ShiftRows<id>(subbyte_out, shiftrows_out);
    MixColumns<id>(shiftrows_out,mixcolumns_out);
    

    key_generation<id-1>(key_in,new_key);
    split<id+1>(new_key,key_to_xor,key_out);
    XorRoundKey<id+1>(mixcolumns_out,key_to_xor,round_out); 
        
    
}

// #pragma hls_design top
void encrypt_hier_block(ac_int<8,false> state_array[4][4],ac_int<128,false> &Rkey,ac_channel<int> &comp_out){
    ac_channel<int>  data_in_state,data_in_key,mixcolumns_out,subbyte_out, shiftrows_out,  res, xorroundkey_out, round1_out,round2_out,round3_out,round4_out,round5_out,round6_out,round7_out,round8_out,round9_out;
    ac_channel<int> key1,key2,key3,key4,key5,key6,key7,key8,key9,key10,key_to_xor,key_to_round;
    ac_int<32,false> data_in;

    int part = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            data_in.set_slc(8*j,state_array[j][i]);
            
        }
        data_in_state.write(data_in);
        data_in_key.write(Rkey.slc<32>(i*32));

    }
    split<1>(data_in_key,key_to_xor,key_to_round);
     
    XorRoundKey<1>(data_in_state,key_to_xor,xorroundkey_out);
    
    round<1,1>(xorroundkey_out,round1_out,key_to_round,key1);
    round<2,2>(round1_out,round2_out,key1,key2); 
    round<3,3>(round2_out,round3_out,key2,key3); 
    round<4,4>(round3_out,round4_out,key3,key4); 
    round<5,5>(round4_out,round5_out,key4,key5); 
    round<6,6>(round5_out,round6_out,key5,key6); 
    round<7,7>(round6_out,round7_out,key6,key7); 
    round<8,8>(round7_out,round8_out,key7,key8); 
    round<9,9>(round8_out,round9_out,key8,key9);  
    
    SubBytes<10>(round9_out,subbyte_out); 
    ShiftRows<10>(subbyte_out, shiftrows_out);
    key_generation<9>(key9,key10);
    XorRoundKey<11>(shiftrows_out,key10,comp_out);
    //print_channel(comp_out);
    
         
}

int main() {
    FILE *f;
    ac_int<8,false> num[80000];
    int i=0;
    int rv;
    int num_values;
    int size = 7;

    ac_int<128,false> Rkey;
    ac_int<128,false> block ;

    ac_int<128,false> Computed_output ;
    int iter=0;
    int part = 0;
    int count[8000] = {0};
    ac_int<8,false> plain_text[16];
    ac_int<8,false> rkey[16];
    ac_int<8,false> out_tmp[16];
    ac_int<8,false> state_array[4][4];

    ac_int<32,false> data_in,out;
    ac_int<128,false> Output_tmp;
    ac_channel<ac_int<128,false>> Output;
    ac_channel<ac_int<8,false>> subbytes_out;
    ac_channel<ac_int<1,false>> ready,ready_out;
    ac_channel<ac_int<32,false>> col,data_in_state,data_in_key,shiftrows_out;
    ac_channel<int>comp_out;

    ac_int<32,false> temp_col;
    ac_int<8,false> tmp_state_array[4][4];

    int error_iter[8000];

    ac_channel<ac_int<128,false>> error;

    error.write(0);

    f=fopen("ECBGFSbox128_7.txt","r"); //CHANGE SIZE LIMIT @LINE 210
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
            Rkey.set_slc(120-i*8,rkey[i]);
        }

        // std::cout<<std::hex<<Rkey<<std::endl;

        // std::cout<<"State Array: "<<std::endl;
        for(int j=0; j<16; j++){
            plain_text[j] = count[i]<100?num[i*49+17+j]:num[i*49+17+j+(i-99)];
        }
        for(int j = 0; j < 4; j++){
            for(int i = 0; i < 4; i++){
            state_array[i][j]=plain_text[part];    
            part++;
            // std::cout<<std::hex<<state_array[i][j];
            }
            // std::cout<<std::endl;
        }

        // std::cout<<"Output:          ";
        for(int j=0; j<16; j++){
            out_tmp[j] = count[i]<100?num[i*49+33+j]:num[i*49+33+j+(i-99)];
        }
        for(int i = 0; i < 16; i++){
            Output_tmp.set_slc(i*8,out_tmp[15-i]);
        }
        Output.write(Output_tmp);
        // std::cout<<std::hex<<Output<<std::endl;

        encrypt_hier_block(state_array,Rkey,comp_out);
        // std::cout<<std::hex<<Rkey<<std::endl;

        if(comp_out.available(4)){
            for(int i = 0; i < 4; i++){
                temp_col = comp_out.read();
                for (int j = 0; j < 4; j++){
                    tmp_state_array[j][i] = temp_col.slc<8>(j*8);
                }
            }
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    Computed_output.set_slc(120-(i*32+j*8),tmp_state_array[j][i]);
                }
            }
            std::cout<<std::hex<<Computed_output<<std::endl;
            if(Output.available(1)){
                if(Output.read()!=Computed_output){
                    error_iter[count[i]] = count[i]+1;
                    iter++;
                }else
                {
                    error_iter[count[i]] = 0;
                }
            }
        }
    }

    std::cout<<"Errors: "<<iter<<std::endl;
    std::cout<<"Error: "<<double(iter*100/size)<<"%"<<std::endl;

    return 0;
}