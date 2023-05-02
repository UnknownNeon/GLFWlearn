#include "glwindow.h"

int main(void)
{
   
    glWindow w;
    GLFWwindow * window = w.makeWindowW("TestWIndow", 600, 699, WINDOWED ,true);
   
    
    float positions[] = {
     0.5f,  0.5f,  // top right
     0.5f, -0.5f,  // bottom right
    -0.5f, -0.5f,  // bottom left
    -0.5f,  0.5f  // top left 
    };

    unsigned int U_positions[] = {  // note that we start from 0!
       0, 1, 2,   // first triangle
       2, 3, 0    // second triangle
    };
 
    unsigned int id = w.GLvertexIndexBuffers(positions, 1, GL_ARRAY_BUFFER, 8);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    GLuint ibo = w.GLvertexIndexBuffers(U_positions, 1, GL_ELEMENT_ARRAY_BUFFER, 6);

    unsigned int program = w.CreateShader("Dependencies/Shaders/Shader.shader");
    glUseProgram(program);

    int location = glGetUniformLocation(program, "U_Color");
   // glUniform4f(location, 0.0, 1.0, 0.0, 1.0);


    float r = 0.0f;
    float inc = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        w.pollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        if (r >= 1.0f)
            inc -= 0.1;
        else if (r <= 0.0f)
            inc += 0.1;
        r += inc;
       
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); 

        glfwSwapBuffers(window);

    }

    return 0;
}
