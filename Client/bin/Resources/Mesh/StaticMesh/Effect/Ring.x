xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Material Material__111 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "eye_zun_t_ring05-2_A.tga";
 }
}

Frame Cylinder01 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh  {
  180;
  25.693190;8.306469;44.502022;,
  15.879253;8.306470;48.871453;,
  26.190352;-6.771451;45.363133;,
  26.190352;-6.771451;45.363133;,
  35.049546;-6.771453;38.926566;,
  25.693190;8.306469;44.502022;,
  34.384211;8.306468;38.187638;,
  25.693190;8.306469;44.502022;,
  35.049546;-6.771453;38.926566;,
  35.049546;-6.771453;38.926566;,
  42.376909;-6.771454;30.788726;,
  34.384211;8.306468;38.187638;,
  41.572483;8.306466;30.204272;,
  34.384211;8.306468;38.187638;,
  42.376909;-6.771454;30.788726;,
  42.376909;-6.771454;30.788726;,
  47.852205;-6.771455;21.305275;,
  41.572483;8.306466;30.204272;,
  46.943843;8.306466;20.900843;,
  41.572483;8.306466;30.204272;,
  47.852205;-6.771455;21.305275;,
  47.852205;-6.771455;21.305275;,
  51.236131;-6.771455;10.890682;,
  46.943843;8.306466;20.900843;,
  50.263536;8.306465;10.683947;,
  46.943843;8.306466;20.900843;,
  51.236131;-6.771455;10.890682;,
  51.236131;-6.771455;10.890682;,
  52.380797;-6.771455;0.000000;,
  50.263536;8.306465;10.683947;,
  51.386470;8.306465;0.000000;,
  50.263536;8.306465;10.683947;,
  52.380797;-6.771455;0.000000;,
  52.380797;-6.771455;0.000000;,
  51.236154;-6.771455;-10.890581;,
  51.386470;8.306465;0.000000;,
  16.186515;-6.771450;49.817117;,
  26.190352;-6.771451;45.363133;,
  15.879253;8.306470;48.871453;,
  15.879253;8.306470;48.871453;,
  5.371319;8.306472;51.104973;,
  16.186515;-6.771450;49.817117;,
  5.475252;-6.771448;52.093856;,
  16.186515;-6.771450;49.817117;,
  5.371319;8.306472;51.104973;,
  5.371319;8.306472;51.104973;,
  -5.371367;8.306474;51.104973;,
  5.475252;-6.771448;52.093856;,
  -5.475305;-6.771446;52.093853;,
  5.475252;-6.771448;52.093856;,
  -5.371367;8.306474;51.104973;,
  -5.371367;8.306474;51.104973;,
  -15.879298;8.306476;48.871437;,
  -5.475305;-6.771446;52.093853;,
  -16.186565;-6.771444;49.817101;,
  -5.475305;-6.771446;52.093853;,
  -15.879298;8.306476;48.871437;,
  -15.879298;8.306476;48.871437;,
  -25.693232;8.306477;44.502000;,
  -16.186565;-6.771444;49.817101;,
  -26.190397;-6.771443;45.363110;,
  -16.186565;-6.771444;49.817101;,
  -25.693232;8.306477;44.502000;,
  -25.693232;8.306477;44.502000;,
  -34.384259;8.306478;38.187600;,
  -26.190397;-6.771443;45.363110;,
  -35.049600;-6.771441;38.926525;,
  -26.190397;-6.771443;45.363110;,
  -34.384259;8.306478;38.187600;,
  -34.384259;8.306478;38.187600;,
  -41.572528;8.306479;30.204218;,
  -35.049600;-6.771441;38.926525;,
  -42.376960;-6.771441;30.788669;,
  -35.049600;-6.771441;38.926525;,
  -41.572528;8.306479;30.204218;,
  -41.572528;8.306479;30.204218;,
  -46.943878;8.306479;20.900766;,
  -42.376960;-6.771441;30.788669;,
  -47.852246;-6.771440;21.305197;,
  -42.376960;-6.771441;30.788669;,
  -46.943878;8.306479;20.900766;,
  -46.943878;8.306479;20.900766;,
  -50.263555;8.306481;10.683853;,
  -47.852246;-6.771440;21.305197;,
  -51.236160;-6.771440;10.890586;,
  -47.852246;-6.771440;21.305197;,
  -50.263555;8.306481;10.683853;,
  -50.263555;8.306481;10.683853;,
  -51.386470;8.306481;0.000005;,
  -51.236160;-6.771440;10.890586;,
  -52.380806;-6.771440;0.000005;,
  -51.236160;-6.771440;10.890586;,
  -51.386470;8.306481;0.000005;,
  -51.386470;8.306481;0.000005;,
  -50.263555;8.306481;-10.683845;,
  -52.380806;-6.771440;0.000005;,
  -51.236160;-6.771440;-10.890578;,
  -52.380806;-6.771440;0.000005;,
  -50.263555;8.306481;-10.683845;,
  -50.263555;8.306481;-10.683845;,
  -46.943882;8.306479;-20.900757;,
  -51.236160;-6.771440;-10.890578;,
  -47.852250;-6.771440;-21.305187;,
  -51.236160;-6.771440;-10.890578;,
  -46.943882;8.306479;-20.900757;,
  -46.943882;8.306479;-20.900757;,
  -41.572531;8.306479;-30.204208;,
  -47.852250;-6.771440;-21.305187;,
  -42.376963;-6.771441;-30.788659;,
  -47.852250;-6.771440;-21.305187;,
  -41.572531;8.306479;-30.204208;,
  -41.572531;8.306479;-30.204208;,
  -34.384262;8.306478;-38.187590;,
  -42.376963;-6.771441;-30.788659;,
  -35.049603;-6.771441;-38.926522;,
  -42.376963;-6.771441;-30.788659;,
  -34.384262;8.306478;-38.187590;,
  -34.384262;8.306478;-38.187590;,
  -25.693237;8.306477;-44.501984;,
  -35.049603;-6.771441;-38.926522;,
  -26.190405;-6.771443;-45.363101;,
  -35.049603;-6.771441;-38.926522;,
  -25.693237;8.306477;-44.501984;,
  -25.693237;8.306477;-44.501984;,
  -15.879298;8.306476;-48.871437;,
  -26.190405;-6.771443;-45.363101;,
  -16.186563;-6.771444;-49.817101;,
  -26.190405;-6.771443;-45.363101;,
  -15.879298;8.306476;-48.871437;,
  -15.879298;8.306476;-48.871437;,
  -5.371351;8.306474;-51.104966;,
  -16.186563;-6.771444;-49.817101;,
  -5.475289;-6.771446;-52.093853;,
  -16.186563;-6.771444;-49.817101;,
  -5.371351;8.306474;-51.104966;,
  -5.371351;8.306474;-51.104966;,
  5.371347;8.306472;-51.104966;,
  -5.475289;-6.771446;-52.093853;,
  5.475280;-6.771448;-52.093853;,
  -5.475289;-6.771446;-52.093853;,
  5.371347;8.306472;-51.104966;,
  5.371347;8.306472;-51.104966;,
  15.879291;8.306470;-48.871437;,
  5.475280;-6.771448;-52.093853;,
  16.186554;-6.771450;-49.817104;,
  5.475280;-6.771448;-52.093853;,
  15.879291;8.306470;-48.871437;,
  15.879291;8.306470;-48.871437;,
  25.693235;8.306469;-44.501990;,
  16.186554;-6.771450;-49.817104;,
  26.190397;-6.771451;-45.363107;,
  16.186554;-6.771450;-49.817104;,
  25.693235;8.306469;-44.501990;,
  25.693235;8.306469;-44.501990;,
  34.384262;8.306468;-38.187590;,
  26.190397;-6.771451;-45.363107;,
  35.049594;-6.771453;-38.926522;,
  26.190397;-6.771451;-45.363107;,
  34.384262;8.306468;-38.187590;,
  34.384262;8.306468;-38.187590;,
  41.572528;8.306466;-30.204211;,
  35.049594;-6.771453;-38.926522;,
  42.376954;-6.771454;-30.788662;,
  35.049594;-6.771453;-38.926522;,
  41.572528;8.306466;-30.204211;,
  41.572528;8.306466;-30.204211;,
  46.943882;8.306466;-20.900763;,
  42.376954;-6.771454;-30.788662;,
  47.852240;-6.771455;-21.305194;,
  42.376954;-6.771454;-30.788662;,
  46.943882;8.306466;-20.900763;,
  46.943882;8.306466;-20.900763;,
  50.263555;8.306465;-10.683848;,
  47.852240;-6.771455;-21.305194;,
  51.236154;-6.771455;-10.890581;,
  47.852240;-6.771455;-21.305194;,
  50.263555;8.306465;-10.683848;,
  50.263555;8.306465;-10.683848;,
  51.386470;8.306465;0.000000;,
  51.236154;-6.771455;-10.890581;;
  60;
  3;0,1,2;,
  3;3,4,5;,
  3;6,7,8;,
  3;9,10,11;,
  3;12,13,14;,
  3;15,16,17;,
  3;18,19,20;,
  3;21,22,23;,
  3;24,25,26;,
  3;27,28,29;,
  3;30,31,32;,
  3;33,34,35;,
  3;36,37,38;,
  3;39,40,41;,
  3;42,43,44;,
  3;45,46,47;,
  3;48,49,50;,
  3;51,52,53;,
  3;54,55,56;,
  3;57,58,59;,
  3;60,61,62;,
  3;63,64,65;,
  3;66,67,68;,
  3;69,70,71;,
  3;72,73,74;,
  3;75,76,77;,
  3;78,79,80;,
  3;81,82,83;,
  3;84,85,86;,
  3;87,88,89;,
  3;90,91,92;,
  3;93,94,95;,
  3;96,97,98;,
  3;99,100,101;,
  3;102,103,104;,
  3;105,106,107;,
  3;108,109,110;,
  3;111,112,113;,
  3;114,115,116;,
  3;117,118,119;,
  3;120,121,122;,
  3;123,124,125;,
  3;126,127,128;,
  3;129,130,131;,
  3;132,133,134;,
  3;135,136,137;,
  3;138,139,140;,
  3;141,142,143;,
  3;144,145,146;,
  3;147,148,149;,
  3;150,151,152;,
  3;153,154,155;,
  3;156,157,158;,
  3;159,160,161;,
  3;162,163,164;,
  3;165,166,167;,
  3;168,169,170;,
  3;171,172,173;,
  3;174,175,176;,
  3;177,178,179;;

  MeshNormals  {
   180;
   0.498915;0.065803;0.864149;,
   0.308346;0.065803;0.948996;,
   0.498915;0.065803;0.864149;,
   0.498915;0.065803;0.864149;,
   0.667680;0.065803;0.741535;,
   0.498915;0.065803;0.864149;,
   0.667680;0.065803;0.741535;,
   0.498915;0.065803;0.864149;,
   0.667680;0.065803;0.741535;,
   0.667680;0.065803;0.741535;,
   0.807263;0.065803;0.586512;,
   0.667680;0.065803;0.741535;,
   0.807263;0.065803;0.586513;,
   0.667680;0.065803;0.741535;,
   0.807263;0.065803;0.586512;,
   0.807263;0.065803;0.586512;,
   0.911565;0.065803;0.405857;,
   0.807263;0.065803;0.586513;,
   0.911565;0.065803;0.405857;,
   0.807263;0.065803;0.586513;,
   0.911565;0.065803;0.405857;,
   0.911565;0.065803;0.405857;,
   0.976027;0.065803;0.207462;,
   0.911565;0.065803;0.405857;,
   0.976027;0.065803;0.207462;,
   0.911565;0.065803;0.405857;,
   0.976027;0.065803;0.207462;,
   0.976027;0.065803;0.207462;,
   0.997833;0.065803;0.000000;,
   0.976027;0.065803;0.207462;,
   0.997833;0.065803;0.000000;,
   0.976027;0.065803;0.207462;,
   0.997833;0.065803;0.000000;,
   0.997833;0.065803;0.000000;,
   0.976028;0.065803;-0.207461;,
   0.997833;0.065803;0.000000;,
   0.308346;0.065803;0.948996;,
   0.498915;0.065803;0.864149;,
   0.308346;0.065803;0.948996;,
   0.308346;0.065803;0.948996;,
   0.104302;0.065803;0.992366;,
   0.308346;0.065803;0.948996;,
   0.104301;0.065803;0.992366;,
   0.308346;0.065803;0.948996;,
   0.104302;0.065803;0.992366;,
   0.104302;0.065803;0.992366;,
   -0.104302;0.065803;0.992366;,
   0.104301;0.065803;0.992366;,
   -0.104302;0.065803;0.992366;,
   0.104301;0.065803;0.992366;,
   -0.104302;0.065803;0.992366;,
   -0.104302;0.065803;0.992366;,
   -0.308347;0.065803;0.948995;,
   -0.104302;0.065803;0.992366;,
   -0.308347;0.065803;0.948995;,
   -0.104302;0.065803;0.992366;,
   -0.308347;0.065803;0.948995;,
   -0.308347;0.065803;0.948995;,
   -0.498916;0.065803;0.864149;,
   -0.308347;0.065803;0.948995;,
   -0.498916;0.065803;0.864149;,
   -0.308347;0.065803;0.948995;,
   -0.498916;0.065803;0.864149;,
   -0.498916;0.065803;0.864149;,
   -0.667680;0.065803;0.741534;,
   -0.498916;0.065803;0.864149;,
   -0.667680;0.065803;0.741534;,
   -0.498916;0.065803;0.864149;,
   -0.667680;0.065803;0.741534;,
   -0.667680;0.065803;0.741534;,
   -0.807264;0.065803;0.586511;,
   -0.667680;0.065803;0.741534;,
   -0.807264;0.065803;0.586511;,
   -0.667680;0.065803;0.741534;,
   -0.807264;0.065803;0.586511;,
   -0.807264;0.065803;0.586511;,
   -0.911566;0.065803;0.405855;,
   -0.807264;0.065803;0.586511;,
   -0.911565;0.065803;0.405855;,
   -0.807264;0.065803;0.586511;,
   -0.911566;0.065803;0.405855;,
   -0.911566;0.065803;0.405855;,
   -0.976028;0.065803;0.207461;,
   -0.911565;0.065803;0.405855;,
   -0.976028;0.065804;0.207461;,
   -0.911565;0.065803;0.405855;,
   -0.976028;0.065803;0.207461;,
   -0.976028;0.065803;0.207461;,
   -0.997833;0.065804;0.000000;,
   -0.976028;0.065804;0.207461;,
   -0.997833;0.065804;-0.000000;,
   -0.976028;0.065804;0.207461;,
   -0.997833;0.065804;0.000000;,
   -0.997833;0.065804;0.000000;,
   -0.976028;0.065804;-0.207461;,
   -0.997833;0.065804;-0.000000;,
   -0.976028;0.065803;-0.207461;,
   -0.997833;0.065804;-0.000000;,
   -0.976028;0.065804;-0.207461;,
   -0.976028;0.065804;-0.207461;,
   -0.911566;0.065803;-0.405855;,
   -0.976028;0.065803;-0.207461;,
   -0.911566;0.065803;-0.405855;,
   -0.976028;0.065803;-0.207461;,
   -0.911566;0.065803;-0.405855;,
   -0.911566;0.065803;-0.405855;,
   -0.807264;0.065803;-0.586511;,
   -0.911566;0.065803;-0.405855;,
   -0.807264;0.065803;-0.586511;,
   -0.911566;0.065803;-0.405855;,
   -0.807264;0.065803;-0.586511;,
   -0.807264;0.065803;-0.586511;,
   -0.667680;0.065803;-0.741534;,
   -0.807264;0.065803;-0.586511;,
   -0.667680;0.065803;-0.741534;,
   -0.807264;0.065803;-0.586511;,
   -0.667680;0.065803;-0.741534;,
   -0.667680;0.065803;-0.741534;,
   -0.498916;0.065803;-0.864148;,
   -0.667680;0.065803;-0.741534;,
   -0.498916;0.065803;-0.864148;,
   -0.667680;0.065803;-0.741534;,
   -0.498916;0.065803;-0.864148;,
   -0.498916;0.065803;-0.864148;,
   -0.308347;0.065803;-0.948995;,
   -0.498916;0.065803;-0.864148;,
   -0.308347;0.065803;-0.948995;,
   -0.498916;0.065803;-0.864148;,
   -0.308347;0.065803;-0.948995;,
   -0.308347;0.065803;-0.948995;,
   -0.104302;0.065803;-0.992366;,
   -0.308347;0.065803;-0.948995;,
   -0.104302;0.065804;-0.992366;,
   -0.308347;0.065803;-0.948995;,
   -0.104302;0.065803;-0.992366;,
   -0.104302;0.065803;-0.992366;,
   0.104302;0.065803;-0.992366;,
   -0.104302;0.065804;-0.992366;,
   0.104302;0.065803;-0.992366;,
   -0.104302;0.065804;-0.992366;,
   0.104302;0.065803;-0.992366;,
   0.104302;0.065803;-0.992366;,
   0.308347;0.065803;-0.948995;,
   0.104302;0.065803;-0.992366;,
   0.308347;0.065803;-0.948995;,
   0.104302;0.065803;-0.992366;,
   0.308347;0.065803;-0.948995;,
   0.308347;0.065803;-0.948995;,
   0.498916;0.065803;-0.864148;,
   0.308347;0.065803;-0.948995;,
   0.498916;0.065803;-0.864148;,
   0.308347;0.065803;-0.948995;,
   0.498916;0.065803;-0.864148;,
   0.498916;0.065803;-0.864148;,
   0.667680;0.065803;-0.741534;,
   0.498916;0.065803;-0.864148;,
   0.667680;0.065803;-0.741534;,
   0.498916;0.065803;-0.864148;,
   0.667680;0.065803;-0.741534;,
   0.667680;0.065803;-0.741534;,
   0.807264;0.065803;-0.586511;,
   0.667680;0.065803;-0.741534;,
   0.807264;0.065803;-0.586511;,
   0.667680;0.065803;-0.741534;,
   0.807264;0.065803;-0.586511;,
   0.807264;0.065803;-0.586511;,
   0.911566;0.065803;-0.405855;,
   0.807264;0.065803;-0.586511;,
   0.911566;0.065803;-0.405855;,
   0.807264;0.065803;-0.586511;,
   0.911566;0.065803;-0.405855;,
   0.911566;0.065803;-0.405855;,
   0.976028;0.065803;-0.207461;,
   0.911566;0.065803;-0.405855;,
   0.976028;0.065803;-0.207461;,
   0.911566;0.065803;-0.405855;,
   0.976028;0.065803;-0.207461;,
   0.976028;0.065803;-0.207461;,
   0.997833;0.065803;0.000000;,
   0.976028;0.065803;-0.207461;;
   60;
   3;0,1,2;,
   3;3,4,5;,
   3;6,7,8;,
   3;9,10,11;,
   3;12,13,14;,
   3;15,16,17;,
   3;18,19,20;,
   3;21,22,23;,
   3;24,25,26;,
   3;27,28,29;,
   3;30,31,32;,
   3;33,34,35;,
   3;36,37,38;,
   3;39,40,41;,
   3;42,43,44;,
   3;45,46,47;,
   3;48,49,50;,
   3;51,52,53;,
   3;54,55,56;,
   3;57,58,59;,
   3;60,61,62;,
   3;63,64,65;,
   3;66,67,68;,
   3;69,70,71;,
   3;72,73,74;,
   3;75,76,77;,
   3;78,79,80;,
   3;81,82,83;,
   3;84,85,86;,
   3;87,88,89;,
   3;90,91,92;,
   3;93,94,95;,
   3;96,97,98;,
   3;99,100,101;,
   3;102,103,104;,
   3;105,106,107;,
   3;108,109,110;,
   3;111,112,113;,
   3;114,115,116;,
   3;117,118,119;,
   3;120,121,122;,
   3;123,124,125;,
   3;126,127,128;,
   3;129,130,131;,
   3;132,133,134;,
   3;135,136,137;,
   3;138,139,140;,
   3;141,142,143;,
   3;144,145,146;,
   3;147,148,149;,
   3;150,151,152;,
   3;153,154,155;,
   3;156,157,158;,
   3;159,160,161;,
   3;162,163,164;,
   3;165,166,167;,
   3;168,169,170;,
   3;171,172,173;,
   3;174,175,176;,
   3;177,178,179;;
  }

  MeshMaterialList  {
   1;
   60;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0;
   { Material__111 }
  }

  MeshTextureCoords  {
   180;
   0.621395;0.711421;,
   0.575026;0.732179;,
   0.666838;0.790564;,
   0.666838;0.790564;,
   0.723273;0.749336;,
   0.621395;0.711421;,
   0.662458;0.681423;,
   0.621395;0.711421;,
   0.723273;0.749336;,
   0.723273;0.749336;,
   0.769950;0.697211;,
   0.662458;0.681423;,
   0.696421;0.643495;,
   0.662458;0.681423;,
   0.769950;0.697211;,
   0.769950;0.697211;,
   0.804829;0.636467;,
   0.696421;0.643495;,
   0.721800;0.599296;,
   0.696421;0.643495;,
   0.804829;0.636467;,
   0.804829;0.636467;,
   0.826385;0.569758;,
   0.721800;0.599296;,
   0.737485;0.550758;,
   0.721800;0.599296;,
   0.826385;0.569758;,
   0.826385;0.569758;,
   0.833676;0.500000;,
   0.737485;0.550758;,
   0.742790;0.500000;,
   0.737485;0.550758;,
   0.833676;0.500000;,
   0.833676;0.500000;,
   0.826385;0.430243;,
   0.742790;0.500000;,
   0.603111;0.819093;,
   0.666838;0.790564;,
   0.575026;0.732179;,
   0.575026;0.732179;,
   0.525378;0.742790;,
   0.603111;0.819093;,
   0.534878;0.833676;,
   0.603111;0.819093;,
   0.525378;0.742790;,
   0.525378;0.742790;,
   0.474621;0.742790;,
   0.534878;0.833676;,
   0.465121;0.833676;,
   0.534878;0.833676;,
   0.474621;0.742790;,
   0.474621;0.742790;,
   0.424974;0.732179;,
   0.465121;0.833676;,
   0.396888;0.819093;,
   0.465121;0.833676;,
   0.424974;0.732179;,
   0.424974;0.732179;,
   0.378605;0.711421;,
   0.396888;0.819093;,
   0.333162;0.790564;,
   0.396888;0.819093;,
   0.378605;0.711421;,
   0.378605;0.711421;,
   0.337541;0.681422;,
   0.333162;0.790564;,
   0.276727;0.749336;,
   0.333162;0.790564;,
   0.337541;0.681422;,
   0.337541;0.681422;,
   0.303578;0.643495;,
   0.276727;0.749336;,
   0.230050;0.697210;,
   0.276727;0.749336;,
   0.303578;0.643495;,
   0.303578;0.643495;,
   0.278200;0.599296;,
   0.230050;0.697210;,
   0.195171;0.636466;,
   0.230050;0.697210;,
   0.278200;0.599296;,
   0.278200;0.599296;,
   0.262515;0.550757;,
   0.195171;0.636466;,
   0.173615;0.569757;,
   0.195171;0.636466;,
   0.262515;0.550757;,
   0.262515;0.550757;,
   0.257210;0.500000;,
   0.173615;0.569757;,
   0.166324;0.500000;,
   0.173615;0.569757;,
   0.257210;0.500000;,
   0.257210;0.500000;,
   0.262515;0.449243;,
   0.166324;0.500000;,
   0.173615;0.430243;,
   0.166324;0.500000;,
   0.262515;0.449243;,
   0.262515;0.449243;,
   0.278200;0.400704;,
   0.173615;0.430243;,
   0.195171;0.363534;,
   0.173615;0.430243;,
   0.278200;0.400704;,
   0.278200;0.400704;,
   0.303578;0.356505;,
   0.195171;0.363534;,
   0.230050;0.302790;,
   0.195171;0.363534;,
   0.303578;0.356505;,
   0.303578;0.356505;,
   0.337541;0.318578;,
   0.230050;0.302790;,
   0.276727;0.250664;,
   0.230050;0.302790;,
   0.337541;0.318578;,
   0.337541;0.318578;,
   0.378605;0.288579;,
   0.276727;0.250664;,
   0.333162;0.209436;,
   0.276727;0.250664;,
   0.378605;0.288579;,
   0.378605;0.288579;,
   0.424974;0.267821;,
   0.333162;0.209436;,
   0.396888;0.180907;,
   0.333162;0.209436;,
   0.424974;0.267821;,
   0.424974;0.267821;,
   0.474621;0.257210;,
   0.396888;0.180907;,
   0.465121;0.166324;,
   0.396888;0.180907;,
   0.474621;0.257210;,
   0.474621;0.257210;,
   0.525378;0.257210;,
   0.465121;0.166324;,
   0.534879;0.166324;,
   0.465121;0.166324;,
   0.525378;0.257210;,
   0.525378;0.257210;,
   0.575026;0.267821;,
   0.534879;0.166324;,
   0.603112;0.180907;,
   0.534879;0.166324;,
   0.575026;0.267821;,
   0.575026;0.267821;,
   0.621395;0.288579;,
   0.603112;0.180907;,
   0.666838;0.209436;,
   0.603112;0.180907;,
   0.621395;0.288579;,
   0.621395;0.288579;,
   0.662458;0.318578;,
   0.666838;0.209436;,
   0.723273;0.250664;,
   0.666838;0.209436;,
   0.662458;0.318578;,
   0.662458;0.318578;,
   0.696422;0.356505;,
   0.723273;0.250664;,
   0.769950;0.302790;,
   0.723273;0.250664;,
   0.696422;0.356505;,
   0.696422;0.356505;,
   0.721800;0.400704;,
   0.769950;0.302790;,
   0.804829;0.363534;,
   0.769950;0.302790;,
   0.721800;0.400704;,
   0.721800;0.400704;,
   0.737485;0.449243;,
   0.804829;0.363534;,
   0.826385;0.430243;,
   0.804829;0.363534;,
   0.737485;0.449243;,
   0.737485;0.449243;,
   0.742790;0.500000;,
   0.826385;0.430243;;
  }
 }
}