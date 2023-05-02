#include "glwindow.h"

static shadesrc Shader(const std::string&);

GLFWwindow* glWindow::makeWindowW(const char* name, int height, int width, bool mode, bool fps_60)
{
    // always need to initialize the glfw things
    if (!glfwInit()) {
        std::cout << "glfwinitfailed" << std::endl;
    }

    if(mode == 0)
        this->window = glfwCreateWindow(height, width, name, NULL, NULL);
    else
        this->window = glfwCreateWindow(height, width, name, glfwGetPrimaryMonitor(), NULL);


    std::cout << this->window << std::endl;

    /* Create a windowed mode window and its OpenGL context */

    if (!this->window)
    {
        glfwTerminate();
        std::cout << "Window Initialization failed. " << std::endl;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);


    if (fps_60 == true)
        glfwSwapInterval(1);

    //initialzing glew
    if (glewInit() != GLEW_OK) {
        std::cout << "failed to init glew" << std::endl;
    }

    return this->window;
}

unsigned int glWindow::GLvertexIndexBuffers(void* a, int noOfBuffersToGenerate, unsigned int Type, int noOfElementsInTheArray)
{
    unsigned int id;

    if (Type == GL_ELEMENT_ARRAY_BUFFER) {
        glGenBuffers(noOfBuffersToGenerate,&id);
        glBindBuffer(Type, id);
        glBufferData(Type , noOfElementsInTheArray * sizeof(unsigned int), (unsigned int  * )a , GL_STATIC_DRAW);
    }
    else
    {
        glGenBuffers(1, &id);
        glBindBuffer(Type, id);
        glBufferData(Type, noOfElementsInTheArray * sizeof(float),(float *) a, GL_STATIC_DRAW);
    }
    return id;
}


//to poll the events in the game loop
void glWindow::pollEvents()
{
    glfwPollEvents();


    //Specify to exit full Screen

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

//This is the part of Shader handiling ////////////////////////////////////////////////////

unsigned int compileShader(unsigned int type, std::string& src) {

    unsigned int id = glCreateShader(type); //MAIN LINE OPENGL FUNCTION
    const char* srcC = src.c_str();
    glShaderSource(id, 1, &srcC, nullptr);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {

      int size;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);
      char* error = (char*)alloca(size * sizeof(char));
      glGetShaderInfoLog(id, size, &size, error);
      std::cout << "Failed to compile shader " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
      std::cout << error << std::endl;

      glDeleteShader(id);
      return 0;
    }

    return id;
}


unsigned int glWindow::CreateShader(const char* filepath)
{
        
    unsigned int program = glCreateProgram();
    SHADERSRC src = Shader(filepath);

    //sending to the Opengl function in compileShader

    unsigned int fs = compileShader(GL_VERTEX_SHADER, src.VertexSource);
    unsigned int vs = compileShader(GL_FRAGMENT_SHADER, src.FragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //we Can delete the shader as the binaries are now stored int he program variable

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}

///
/// ////////////////////////////////Shadernnnnnnnnnn///////////////////////////

static shadesrc Shader(const std::string& filepath)
{
    std::cout << filepath << std::endl;
    std::string line;

    std::ifstream stream(filepath);
    if (stream.is_open())
    {
        std::cout << "open" << std::endl;
    }

    enum class shaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    
    std::stringstream ss[2];
    shaderType sh = shaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("fragment") != std::string::npos)
            {
                sh = shaderType::FRAGMENT;
            }
            else if (line.find("vertex") != std::string::npos)
            {
                sh = shaderType::VERTEX;
            }

        }
        else
        {
            ss[(int)sh] << line << '\n';
        }

    }

    return { ss[0].str(), ss[1].str() };
}


glWindow::~glWindow()
{
    glfwTerminate();
}

