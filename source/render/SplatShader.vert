R"(////////// SHADER BEGIN /////////

// Positions
in vec4 aPosition;

// Colors
in  vec4 aColor;
out vec4 outColor;

// Texture coordinates
in  vec2 aTexCoord;
out vec2 outTexCoord;

// Matrices
uniform mat4 uPerspective;

// Transform and pass through
void main(void) {
    gl_Position = uPerspective*aPosition;
    outColor = aColor;
    outTexCoord = aTexCoord;
}

/////////// SHADER END //////////)"


