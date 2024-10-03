#include "glwindow.h"

static shadesrc Shader(const std::string&);

void glWindow::init(const char* name, int height, int width, bool full_screen, bool fps_60)
{
    // always need to initialize the glfw things
    if (!glfwInit()) {
        std::cout << "glfwinitfailed" << std::endl;
    }

    if(!full_screen)
        this->instance_pointer = glfwCreateWindow(width, height, name, NULL, NULL);
    else
        this->instance_pointer = glfwCreateWindow(width, height, name, glfwGetPrimaryMonitor(), NULL);


    std::cout << this->instance_pointer << std::endl;

    /* Create a windowed mode window and its OpenGL context */

    if (!this->instance_pointer)
    {
        glfwTerminate();
        std::cout << "Window Initialization failed. " << std::endl;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(this->instance_pointer);

    if (fps_60 == true)
        glfwSwapInterval(1);

    //initialzing glew
    if (glewInit() != GLEW_OK) {
        std::cout << "failed to init glew" << std::endl;
    }

}

//To poll the events in the game loop : INPUT GOES HERE
void glWindow::pollEvents()
{
    glfwPollEvents();
    //Specify to exit full Screen
    if (glfwGetKey(this->instance_pointer, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->instance_pointer, GL_TRUE);
}

//This is the part of Shader handiling 
unsigned int compile_shader(unsigned int type, std::string& src) {

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
unsigned int glWindow::create_shader(const char* filepath)
{
        
    unsigned int program = glCreateProgram();
    SHADERSRC src = Shader(filepath);

    //sending to the Opengl function in compileShader
    unsigned int fs = compile_shader(GL_VERTEX_SHADER, src.VertexSource);
    unsigned int vs = compile_shader(GL_FRAGMENT_SHADER, src.FragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //we Can delete the shader as the binaries are now stored int he program variable

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}
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


void glWindow::loop()
{
    while (!glfwWindowShouldClose(this->instance_pointer)) {

        this->pollEvents(); 
        glBindVertexArray(this->vao);

        matrices.rotate_model(glm::radians(0.3f),0.3,1,0);
        matrices.config(this->shader_program_id);

        this->push_dbuffer(this->render_objects.size());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, this->render_objects.size());

        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(this->instance_pointer);

    }
}

void glWindow::add_to_scene(std::vector<Vertex> v)
{
    this->object_manager.push_back(v);
    this->bake();
}

void glWindow::bake()
{
    this->render_objects.clear();

    for (int i = 0; i < object_manager.size(); i++) {
        for (int j = 0; j < object_manager[i].size(); j++) {
            this->render_objects.push_back(object_manager[i][j]);
        
        }
    }
}

void glWindow::translate_object_and_add(float x, float y, float z , std::vector<Vertex> v)
{
    for (int i = 0; i < v.size(); i++) {
        v[i].position =  glm::vec3((glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * glm::vec4(v[i].position, 1.0)));
    }
    this->add_to_scene(v);
}

void glWindow::translate_object(float x, float y, float z ,int item_number)
{
    std::vector<Vertex> v = object_manager[item_number];

    for (int i = 0; i < v.size(); i++) {
        v[i].position = glm::vec3((glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * glm::vec4(v[i].position, 1.0)));
    }
    this->object_manager[item_number] = v;
    this->bake();
}

//Create Buffers Section
unsigned int glWindow::create_buffers(void* a, unsigned int Type, int noOfElementsInTheArray)
{
    unsigned int id;

    if (Type == GL_ELEMENT_ARRAY_BUFFER) {
        glGenBuffers(1, &id);
        glBindBuffer(Type, id);
        glBufferData(Type, noOfElementsInTheArray * sizeof(unsigned int), (unsigned int*)a, GL_STATIC_DRAW); 
    }
    else
    {
        std::cout << noOfElementsInTheArray << std::endl;
        //For Vertex Buffer
        glGenBuffers(1, &id);
        glBindBuffer(Type, id);
        glBufferData(Type, noOfElementsInTheArray * sizeof(float), (float*)a, GL_STATIC_DRAW);
        // Args are : Index , size per vertex , type , normalized , stride , pointer 
        glVertexAttribPointer(this->vcount, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(this->vcount);
        vcount++;
        glVertexAttribPointer(this->vcount, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(this->vcount);
        vcount++;
    }
    return id;
}

unsigned int glWindow::create_dynamic_buffer(unsigned int type,unsigned int elements_in_array)
{
    unsigned int id;
    glGenBuffers(1, &id);
    glBindBuffer(type, id);
    glBufferData(type, (elements_in_array * sizeof(Vertex)),nullptr, GL_DYNAMIC_DRAW);

    //Structure of the DATA
    // Args are : Index , size per vertex , type , normalized , stride , pointer 
    glVertexAttribPointer(this->vcount, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(this->vcount);
    vcount++;
    glVertexAttribPointer(this->vcount, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(this->vcount);
    vcount++;
    glVertexAttribPointer(this->vcount, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(this->vcount);
    vcount++;
    
    return id;
}

void glWindow::push_dbuffer(unsigned int elements_in_array)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->DynamicVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, elements_in_array * sizeof(Vertex), this->render_objects.data());
}

void glWindow::record_vao()
{
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
}
void glWindow::end_record_vao()
{
    glBindVertexArray(0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void glWindow::init()
{
    this->init("TestWIndow", HEIGHT, WIDTH, false, true);
    
    this->add_to_scene(this->vertices);
    /*this->translate_object_and_add(1.5f, 2.2f, 2.5f, vertices);
    this->translate_object(-1.5f, -2.2f, -2.5f, 0);*/

    this->record_vao();
    this->DynamicVBO = this->create_dynamic_buffer(GL_ARRAY_BUFFER, this->render_objects.size()); //Fixing 
    this->create_buffers(this->indices.data(),GL_ELEMENT_ARRAY_BUFFER, this->indices.size());
    this->end_record_vao();

    this->shader_program_id = this->create_shader("Dependencies/Shaders/Shader.shader");

    //The MVP matrices 
    matrices.translate_view(0.0f, 0.0f, -20.0f);
    matrices.prespective_mode(45.f,0.1,100.f);
    //matrices.orthographic_mode(-WIDTH, WIDTH, -HEIGHT, HEIGHT);
    //matrices.scale_model(0.5,0.5,0.5); //Scaling 500 TImes


    //set wireframemode here:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shader_program_id);

    this->loop();
}

//Constructor and destructor side 
glWindow::glWindow()
{
    
    std::string loc = "Dependencies/Shaders/BootsForExport.obj";
    loader.loadObject(loc);

    this->vertices = loader.Object;
    this->indices = loader.vertex_indices;

    this->vcount = 0;
    this->init();
}

glWindow::~glWindow()
{
    //Need to free Buffers 
    glfwTerminate();
}
