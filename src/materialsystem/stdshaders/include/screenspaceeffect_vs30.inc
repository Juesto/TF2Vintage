// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH

#ifndef SCREENSPACEEFFECT_VS30_H
#define SCREENSPACEEFFECT_VS30_H

#include "shaderapi/ishaderapi.h"
#include "shaderapi/ishadershadow.h"
#include "materialsystem/imaterialvar.h"

class screenspaceeffect_vs30_Static_Index
{
	unsigned int m_nX360APPCHOOSER : 2;
public:
	void SetX360APPCHOOSER( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nX360APPCHOOSER = i;
	}

	screenspaceeffect_vs30_Static_Index( IShaderShadow* pShaderShadow, IMaterialVar** params )
	{
		m_nX360APPCHOOSER = 0;
	}

	int GetIndex() const
	{
		return ( 1 * m_nX360APPCHOOSER ) + 0;
	}
};

#define shaderStaticTest_screenspaceeffect_vs30 1


class screenspaceeffect_vs30_Dynamic_Index
{
public:
	screenspaceeffect_vs30_Dynamic_Index( IShaderDynamicAPI* pShaderAPI )
	{
	}

	int GetIndex() const
	{
		return 0;
	}
};

#define shaderDynamicTest_screenspaceeffect_vs30 1


#endif	// SCREENSPACEEFFECT_VS30_H