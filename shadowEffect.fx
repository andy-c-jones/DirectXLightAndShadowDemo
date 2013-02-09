float4x4 worldMat; 
float4x4 worldViewProjMat;
textureCUBE cubeShadowMap; 

const float4 materialAmbient = float4(0.9f, 0.9f, 0.9f, 1.0f);  
const float4 materialDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
const float4 materialSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
const float4 globalAmbient = float4(1.0f, 1.0f, 1.0f, 1.0f);

const float4 lightDiffuse = float4(0.7f, 0.7f, 0.7f, 1.0f);
const float4 lightSpecular = float4(0.3f, 0.3f, 0.3f, 1.0f);
const float4 lightAttenuation = float4(0.0f, 0.05f, 0.0f, 1.0f);

const float specPower = 64.0f;

int lightNumber;
float4 lightPosition[1];
float4 eyePosition;

#define zOffset 0.5f 

samplerCUBE cubeShadowMapSampler = sampler_state
{
	Texture = <cubeShadowMap>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = wrap;
    AddressV = wrap;
};

struct lightFuncOutput
{
    float4 diffuseResult[1];
    float4 specularResult[1];
};

lightFuncOutput LightPointSH(float3 inObjPos, 
						     float3 inNormal, 
						     float3 inCam2Vertex)
{
     lightFuncOutput output;
     output.diffuseResult[0] = float4(0.0f, 0.0f, 0.0f, 1.0f);
     output.specularResult[0] = float4(0.0f, 0.0f, 0.0f, 1.0f);

     float4 PLightDirection = 0.0f;
     PLightDirection.xyz = lightPosition[0].xyz - inObjPos;
     float distance = length(PLightDirection.xyz);
     PLightDirection.xyz = PLightDirection.xyz / distance;
    
     PLightDirection.w = max(0, 1 / (lightAttenuation.x + 
     	                      		 lightAttenuation.y * distance + 
                               		 lightAttenuation.z * distance * distance) );
     
	 float shadowMapDepth = texCUBE(cubeShadowMapSampler, float4(-(PLightDirection.xyz), 0.0f)).x;
	 
	 if(distance > shadowMapDepth)    
	 {
	 	 return output;
	 }     	
	 else
	 {
	     float3 floatVecTmp = normalize(inCam2Vertex + PLightDirection.xyz);

         output.diffuseResult[0] = PLightDirection.w * lightDiffuse * max(0, dot(inNormal, PLightDirection.xyz));
         output.specularResult[0] = PLightDirection.w * lightSpecular * pow(max (0, dot(inNormal, floatVecTmp) ), specPower);

         return output;
	 }
}

struct VS_OUTPUT
{
	float4 position  :  POSITION;
	float3 worldPos  :  TEXCOORD0;
	float3 normalW   :  TEXCOORD1;
	float3 cam2Vert  :  TEXCOORD2;
};

struct VS_OUTPUT_DEPTH
{
    float4 oPositionLight : POSITION;
    float3 lightVec       : TEXCOORD0;
};

VS_OUTPUT_DEPTH depthMap_VS( float4 inPosition : POSITION )
{
    VS_OUTPUT_DEPTH output;
    
    float4 positionW = mul( inPosition, worldMat );
    output.oPositionLight = mul( inPosition, worldViewProjMat );
    
    output.lightVec = lightPosition[0] - positionW.xyz; 

    return output;
}

VS_OUTPUT cubicShadowMapping_VS(float4 inPosition  : POSITION,
                                float3 inNormal    : NORMAL)
                         
{
    VS_OUTPUT output;

	float4 positionW = mul(inPosition, worldMat);

    output.cam2Vert = (eyePosition - positionW).xyz; 
    output.position = mul(inPosition, worldViewProjMat);
    output.worldPos = positionW.xyz;
    output.normalW = mul(inNormal, worldMat).xyz;
    
    return output;
}

float4 ambient_VS(float4 inPosition : POSITION) : POSITION
{
	return mul(inPosition, worldViewProjMat);
}

float4 depthMap_PS( VS_OUTPUT_DEPTH In ) : COLOR0
{
    return length(In.lightVec) + zOffset;
}

float4 cubicShadowMapping_PS(VS_OUTPUT In) : COLOR0
{
    lightFuncOutput lightResult;
    
    float3 normal = normalize(In.normalW);
    float3 cam2Vert = normalize(In.cam2Vert);
    
    lightResult = LightPointSH(In.worldPos, normal, cam2Vert);
    
    float4 ambient = materialAmbient * globalAmbient;
    float4 diffuse = materialDiffuse * lightResult.diffuseResult[0];
    float4 specular = materialSpecular * lightResult.specularResult[0];

    float4 lightingColour = (ambient * (diffuse + specular));
    
    return lightingColour;
}

float4 ambient_PS(float4 posWVP : POSITION) : COLOR0
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f); 
}

technique depthMap
{
    pass P0
    {          
        VertexShader = compile vs_2_0 depthMap_VS( );
        PixelShader  = compile ps_2_0 depthMap_PS( ); 
    }
}

technique cubicShadowMapping
{
    pass P0
    {          
        VertexShader = compile vs_2_0 cubicShadowMapping_VS( );
        PixelShader  = compile ps_2_0 cubicShadowMapping_PS( ); 
    }
}

technique ambient
{
    pass P0
    {          
        VertexShader = compile vs_2_0 ambient_VS( );
        PixelShader  = compile ps_2_0 ambient_PS( ); 
    }
}