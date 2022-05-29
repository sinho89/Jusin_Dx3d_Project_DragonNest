
CManagement* Get_Management(void)
{
	return CManagement::GetInstance();
}

CLightMgr* Get_LightMgr(void)
{
	return CLightMgr::GetInstance();
}
CFrustum*	Get_Frustum(void)
{
	return CFrustum::GetInstance();
}
CQuadTree*	Get_QuadTree(void)
{
	return CQuadTree::GetInstance();
}

CNaviMgr* Get_NaviMgr(void)
{
	return CNaviMgr::GetInstance();
}

CShaderMgr* Get_ShaderMgr(void)
{
	return CShaderMgr::GetInstance();
}
CCameraMgr* Get_CameraMgr(void)
{
	return CCameraMgr::GetInstance();
}
CShadowMgr* Get_ShadowMgr(void)
{
	return CShadowMgr::GetInstance();
}