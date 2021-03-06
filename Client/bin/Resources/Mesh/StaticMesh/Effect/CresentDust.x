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


Material Material__94 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "eye_zun_t_smo012_A.tga";
 }
}

Frame EYE_ZUN_CO_hit03 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh  {
  24;
  -67.871078;-5.000000;-55.000000;,
  -67.871072;95.000000;-64.888365;,
  -39.185395;95.000000;-83.189638;,
  -39.185395;-5.000000;-73.301267;,
  -67.871078;-5.000000;-55.000000;,
  -39.185395;95.000000;-83.189638;,
  -39.185395;95.000000;-83.189638;,
  -0.000024;95.000000;-89.888365;,
  -39.185395;-5.000000;-73.301267;,
  -0.000026;-5.000000;-80.000000;,
  -39.185395;-5.000000;-73.301267;,
  -0.000024;95.000000;-89.888365;,
  -0.000024;95.000000;-89.888365;,
  39.185354;95.000000;-83.189645;,
  -0.000026;-5.000000;-80.000000;,
  39.185354;-5.000000;-73.301280;,
  -0.000026;-5.000000;-80.000000;,
  39.185354;95.000000;-83.189645;,
  39.185354;95.000000;-83.189645;,
  67.871053;95.000000;-64.888384;,
  39.185354;-5.000000;-73.301280;,
  67.871046;-5.000000;-55.000019;,
  39.185354;-5.000000;-73.301280;,
  67.871053;95.000000;-64.888384;;
  8;
  3;0,1,2;,
  3;3,4,5;,
  3;6,7,8;,
  3;9,10,11;,
  3;12,13,14;,
  3;15,16,17;,
  3;18,19,20;,
  3;21,22,23;;

  MeshNormals  {
   24;
   -0.535994;-0.083075;-0.840125;,
   -0.535994;-0.083075;-0.840125;,
   -0.354696;-0.092006;-0.930444;,
   -0.363012;-0.091691;-0.927262;,
   -0.535994;-0.083075;-0.840125;,
   -0.354696;-0.092006;-0.930444;,
   -0.354696;-0.092006;-0.930444;,
   -0.000000;-0.098404;-0.995147;,
   -0.363012;-0.091691;-0.927262;,
   -0.000000;-0.098404;-0.995147;,
   -0.363012;-0.091691;-0.927262;,
   -0.000000;-0.098404;-0.995147;,
   -0.000000;-0.098404;-0.995147;,
   0.354696;-0.092006;-0.930444;,
   -0.000000;-0.098404;-0.995147;,
   0.363012;-0.091691;-0.927262;,
   -0.000000;-0.098404;-0.995147;,
   0.354696;-0.092006;-0.930444;,
   0.354696;-0.092006;-0.930444;,
   0.535993;-0.083075;-0.840125;,
   0.363012;-0.091691;-0.927262;,
   0.535993;-0.083075;-0.840125;,
   0.363012;-0.091691;-0.927262;,
   0.535993;-0.083075;-0.840125;;
   8;
   3;0,1,2;,
   3;3,4,5;,
   3;6,7,8;,
   3;9,10,11;,
   3;12,13,14;,
   3;15,16,17;,
   3;18,19,20;,
   3;21,22,23;;
  }

  MeshMaterialList  {
   1;
   8;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0;
   { Material__94 }
  }

  MeshTextureCoords  {
   24;
   0.004824;1.000000;,
   0.004824;0.000000;,
   0.252412;0.000000;,
   0.252412;1.000000;,
   0.004824;1.000000;,
   0.252412;0.000000;,
   0.252412;0.000000;,
   0.500000;0.000000;,
   0.252412;1.000000;,
   0.500000;1.000000;,
   0.252412;1.000000;,
   0.500000;0.000000;,
   0.500000;0.000000;,
   0.747588;0.000000;,
   0.500000;1.000000;,
   0.747588;1.000000;,
   0.500000;1.000000;,
   0.747588;0.000000;,
   0.747588;0.000000;,
   0.995177;0.000000;,
   0.747588;1.000000;,
   0.995177;1.000000;,
   0.747588;1.000000;,
   0.995177;0.000000;;
  }
 }
}