R"(////////// SHADER BEGIN /////////

#ifdef CUGLES
// This one line is all the difference
precision highp float;  // highp required for gradient precision
#endif

// The texture for sampling
uniform sampler2D uTexture;

//TODO: Custom Uniforms
//uniform vec2 uSplatPosition;
uniform vec2 uS1;
uniform vec2 uS2;
uniform vec2 uS3;
uniform vec2 uS4;
uniform vec4 uC1;
uniform vec4 uC2;
uniform vec4 uC3;
uniform vec4 uC4;
uniform vec2 uViewport;


//Values:
//Fractal
const int MAGIC_BOX_ITERS = 11; //Adjust this: higher = more detail, 11
const float MAGIC_BOX_MAGIC = .55; //This number is scary, dont mess with it, .55
//Splatter
float centerThreshold = 1.0; //Closer they are, further it goes?
float edgeThreshold = 100.0;
float splatFalloffSize = .8;
float splatCutoffSize = .2;
vec4 defaultColor = vec4(0,0,0,0);
vec2 iResolution;


// The output color
out vec4 frag_color;

// The inputs from the vertex shader
//in vec2 outPosition;
//in vec4 outColor;
//in vec2 outTexCoord;

// The stroke+gradient uniform block
//layout (std140) uniform uContext
//{
//    mat3 scMatrix;      // 48
//    vec2 scExtent;      //  8
//    vec2 scScale;       //  8
//    mat3 gdMatrix;      // 48
//    vec4 gdInner;       // 16
//    vec4 gdOuter;       // 16
//    vec2 gdExtent;      //  8
//    float gdRadius;     //  4
//    float gdFeathr;     //  4
//};



//Ben's Code Here to main

//Fractal from https://www.shadertoy.com/view/4ljGDd
float magicBox(vec3 p) {
    // The fractal lives in a 1x1x1 box with mirrors on all sides.
    // Take p anywhere in space and calculate the corresponding position
    // inside the box, 0<(x,y,z)<1
    p = 1.0 - abs(1.0 - mod(p, 2.0));
    
    float lastLength = length(p);
    float tot = 0.0;
    // This is the fractal.  More iterations gives a more detailed
    // fractal at the expense of more computation.
    for (int i=0; i < MAGIC_BOX_ITERS; i++) {
      // The number subtracted here is a "magic" paremeter that
      // produces rather different fractals for different values.
      p = abs(p)/(lastLength*lastLength) - MAGIC_BOX_MAGIC;
      float newLength = length(p);
      tot += abs(newLength-lastLength);
      lastLength = newLength;
    }

    return tot;
}

const mat3 M = mat3(0.28862355854826727, 0.6997227302779844, 0.6535170557707412,
                    0.06997493955670424, 0.6653237235314099, -0.7432683571499161,
                    -0.9548821651308448, 0.26025457467376617, 0.14306504491456504);


float euclideanDistance(vec2 a, vec2 b){
    return sqrt(pow((a.x - b.x), 2.0) + pow((a.y - b.y), 2.0));
}

float lerp(float a, float b, float w)
{
  return a + w*(b-a);
}

bool inRange(vec2 a, vec2 b, float maxDist){
    return (euclideanDistance(a, b)/iResolution.y) < maxDist;
}

vec4 overlayColors(vec4 first, vec4 second){
    return first * first.w + second * (1.0 - first.w);
}


vec4 splatColor(vec2 splatCenter, vec4 paintColor, vec2 fragCoord, vec2 uv, float falloffMod)
{

    //FRACTAL FUN:
    // Rotate uv onto the random axes given by M, and scale
    // it down a bit so we aren't looking at the entire
    // 1x1x1 fractal volume.  Making the coefficient smaller
    // "zooms in", which may reduce large-scale repetition
    // but requires more fractal iterations to get the same
    // level of detail.
    vec3 p = 0.3*M*vec3(uv, 0.0);
    
    float result = magicBox(p);
    // Scale to taste.  Also consider non-linear mappings.
    //result *= 0.025;
       //TODO: Scale to dist
        //Removed this since can just scale threshold by distance
        
        
    //Setup Colors (fun)
    //vec3 paintColor = vec3(255.0, 0.0, 0.0);
    //vec3 paintColor = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));

    vec4 retColor;
    
    //Now clamp them values
    //Set threshold to inverse distance
    float dist = euclideanDistance(fragCoord, splatCenter); //Pixels
    float screenDist = dist/iResolution.y; //0 to 1 scale, percent of screen

    //float threshold = centerThreshold + slope * adjDist; //.25
    float threshold = lerp(centerThreshold, edgeThreshold, screenDist/(splatFalloffSize * falloffMod + .1));
    
    //circle
    //if(screenDist/splatCutoffSize > splatCutoffSize){retColor = black;}
    
    if (result > threshold){
        retColor = paintColor;
    }else{
        retColor = defaultColor;
    }
 
    
    return retColor;
}


float closestTOnLine(vec2 p, vec2 a, vec2 b){
    vec2 ap = p - a;
    vec2 ab = b - a;
    //Squared magnitude of ab
    float atb2 = ab.x*ab.x + ab.y*ab.y;
    
    float ap_dot_ab = ap.x*ab.x + ap.y*ab.y;
    
    //Normalized distance from a to closest point
    float t = ap_dot_ab / atb2;

    t = min(max(t,0.0), 1.0);
    
    return t;
}


vec2 closestPointToLine(vec2 p, vec2 a, vec2 b){
    vec2 ap = p - a;
    vec2 ab = b - a;
    float t = closestTOnLine(p, a, b);

    return a + ab * t;
}


vec4 getOneSplat(vec2 splatCenter, vec4 paintColor, vec2 fragCoord, vec2 uv){
    if(inRange(splatCenter, fragCoord, splatCutoffSize)){
        return splatColor(splatCenter, paintColor, fragCoord, uv, 1.0);
    }
    return defaultColor;
}


//Return the color at a point, (0,0,0) alpha 0 by default


//Get color based on distance from two splat centers
//1 - get distance to the line
vec4 getOneStreak(vec2 center1, vec2 center2, vec4 paintColor1, vec4 paintColor2, vec2 fragCoord, vec2 uv){

    //T is distance from A to B  (1 to 2) from 0 to 1
    float t = closestTOnLine(fragCoord, center1, center2);

    vec2 closestPoint = closestPointToLine(fragCoord, center1, center2);

    float alphaLerp = mix(paintColor1.w, paintColor2.w, t);
    float rLerp = mix(paintColor1.x, paintColor2.x, t);
    float gLerp = mix(paintColor1.y, paintColor2.y, t);
    float bLerp = mix(paintColor1.z, paintColor2.z, t);
    
    vec4 mixColor = vec4(rLerp, gLerp, bLerp, alphaLerp);
    
    //Change to 1-t to reverse trail
    float adjFalloff = (t) * alphaLerp;

    if(inRange(closestPoint,fragCoord, splatCutoffSize)){
        //return vec4(255.0, 0.0, 255.0, 1.0);
        return splatColor(closestPoint, mixColor, fragCoord, uv, adjFalloff);
    }

    return defaultColor;
}



vec4 getSplatColorAtPixel(vec2 fragCoord, vec2 uv){
    vec4 retColor = defaultColor;
//    retColor = overlayColors(retColor, getOneSplat(uS1, uC1, fragCoord, uv));
//    retColor = overlayColors(retColor, getOneSplat(uS2, uC2, fragCoord, uv));
//    retColor = overlayColors(retColor, getOneSplat(uS3, uC3, fragCoord, uv));
//    retColor = overlayColors(retColor, getOneSplat(uS4, uC4, fragCoord, uv));
    //retColor = overlayColors(retColor, getOneStreak(uS1, uS2, uC1, uC2, fragCoord, uv));
    
    float lowMargin = 20.0;
    float highMargin = 200.0;
    //Check distance between first two, if very close, do a tap splat just once
    if(euclideanDistance(uS1, uS2) < lowMargin){
        //Call a splat on the first point
//        vec4 solidColor = vec4(uC1.x, uC1.y, uC1.z, 1.0);
        retColor = overlayColors(retColor, getOneSplat(uS1, uC1, fragCoord, uv));
    }
//    else if (euclideanDistance(uS1, uS2) > highMargin){
//            //There is a remainign point far behind
//            retColor = overlayColors(retColor, getOneStreak(uS1, uS2, uC1, uC2, fragCoord, uv));
//    }
    else{
        retColor = overlayColors(retColor, getOneStreak(uS1, uS2, uC1, uC2, fragCoord, uv));
        retColor = overlayColors(retColor, getOneStreak(uS2, uS3, uC2, uC3, fragCoord, uv));
        retColor = overlayColors(retColor, getOneStreak(uS3, uS4, uC3, uC4, fragCoord, uv));
    }

    
    //if dist very large, then set c2,3,4 alpha to 0 on the
 
    
    return retColor;
}





/**
 * Performs the main fragment shading.
 */
void main(void) {
    vec4 result;
    
    //Ben's code here to end:
    iResolution = uViewport;

    // uv are screen coordinates, uniformly scaled to go from 0..1 vertically
    vec2 uv = gl_FragCoord.xy / iResolution.yy;
    
    vec2 uvOffset = vec2(-200,100);
    //Get splats
    vec4 dotColor = getSplatColorAtPixel(gl_FragCoord.xy,(uv)*1.5+uvOffset);
    
    //Alpha blending - Blend of Result from above and splatter
//    result = overlayColors(dotColor, result);
    //alpha combine splatter and result, assuming result wont use alpha
    //result = dotColor.w * dotColor + (1-dotColor.w)*result;
    
    frag_color = dotColor;
}
/////////// SHADER END //////////)"

