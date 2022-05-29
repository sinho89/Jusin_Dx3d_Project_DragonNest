/*!
 * \file Engine_Enum.h
 * \date 2015/04/05 8:42
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum ANGLE {ANGLE_X, ANGLE_Y, ANGLE_Z, ANGLE_END,};
	enum TEXTURETYPE { TEXTURE_NORMAL, TEXTURE_CUBE };
	enum MESHTYPE {MESH_STATIC, MESH_DYNAMIC};
	enum RENDERGROUP {TYPE_PRIORITY, TYPE_NONEALPHA, TYPE_ALPHA, TYPE_UI, TYPE_END};
	enum OBJECTTYPE {OBJ_PLAYER, OBJ_ORGE, OBJ_GOBLIN, OBJ_BOSS, OBJ_SKILL, OBJ_ALPHA, OBJ_END};
	enum LAYERTYPE{ LAYER_ENVI, LAYER_GAME, LAYER_UI, LAYER_END};
	enum COLORTYPE{ COLOR_BLUE, COLOR_RED, COLOR_PURPLE, COLOR_END};
}

#endif // Engine_Enum_h__