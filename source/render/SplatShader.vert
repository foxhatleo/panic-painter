R"(////////// SHADER BEGIN /////////

in vec4 worldPos;
out vec2 outPosition;

// Matrices
uniform vec2 uViewport;


// Transform and pass through
void main(void) {
    float newX = (worldPos.x / uViewport.x) * 2.0f - 1.0f;
    float newY = (worldPos.y / uViewport.y) * 2.0f - 1.0f;
    gl_Position = vec4(newX, newY, 1, 1);
    outPosition = worldPos.xy;
}

/////////// SHADER END //////////)"


