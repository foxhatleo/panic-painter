R"(////////// SHADER BEGIN /////////

in vec4 aPosition;
out vec2 outPosition;

// Matrices
uniform mat4 uPerspective;


// Transform and pass through
void main(void) {
    gl_Position = uPerspective*aPosition;
    outPosition = aPosition.xy;
}

/////////// SHADER END //////////)"


