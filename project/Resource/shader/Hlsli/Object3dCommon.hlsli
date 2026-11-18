
#ifndef OBJECT_FUNCTIONS_HLSLI
#define OBJECT_FUNCTIONS_HLSLI

float4 GetTemperatureTexture(float objectAlpha, float maskVol, float temperature, uint useOutLine)
{
    //OutLineCancel
    float outlineMask = 1.0f - objectAlpha;
    outlineMask = clamp(outlineMask, 0.0f, 1.0f);
    outlineMask = max(outlineMask, maskVol);
    //SetTemperature　Use G Channel for OutlineMask
        // 0ビット目（0b01）の判定
    bool useOutline = (useOutLine & 1u) != 0;
    // 1ビット目（0b10）の判定
    bool useHoverOutline = (useOutLine & (1u << 1)) != 0;
    float val = 1.0f;
    if (useOutLine)
    {
        if (useHoverOutline)
        {
            val = 0.0f;
        }

    }
    
    return float4(temperature, outlineMask, val, 1.0);
};

bool DisCardColor(float textureAlpha, float materialAlpha, float temperatureR)
{
    
    bool isColorAlpha = textureAlpha == 0.0 || temperatureR == 0.0;
   
    return textureAlpha <= 0.0 && isColorAlpha;
}
#endif // OBJECT_FUNCTIONS_HLSLI