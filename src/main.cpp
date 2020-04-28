#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

int width, height;
float rotation_angle = 0.0f;
bool rotation = false, wireframe = false;

std::string read_file(std::string const &name) {
    std::ifstream ifile(name);

    if (ifile) {
        std::string filetext;

        while (ifile.good()) {
            std::string line;
            std::getline(ifile, line);
            filetext.append(line + "\n");
        }

        return filetext;
    } else {
        throw std::invalid_argument(name);
    }
}

// check for shader error; status = GL_LINK_STATUS or GL_COMPILE_STATUS
bool checkShaderError(GLuint shaderID, GLuint status) {
    int succes;
    char log[1024];
    // Get compilation status
    glGetShaderiv(shaderID, status, &succes);
    // Compilation error
    if (!succes) {
        // Gets the error message
        glGetShaderInfoLog(shaderID, 1024, NULL, log);
        std::cout << "Shader ERROR: " << log << std::endl;
        return false;
    }
    return true;
}

// return the ID of the program created
GLuint createShader(std::string const &vertexShaderPath, std::string const &fragmentShaderPath) {
    // creating program, vertex and fragment shaders
    GLuint shaderProgram = glCreateProgram();
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string verShader_source{read_file(vertexShaderPath)};
    const char *vertShader_chars = verShader_source.c_str();
    std::string fragShader_source{read_file(fragmentShaderPath)};
    const char *fragShader_chars = fragShader_source.c_str();

    // associate the shader source code with a shader object
    glShaderSource(vertShader, 1, &vertShader_chars, NULL);
    glShaderSource(fragShader, 1, &fragShader_chars, NULL);

    // compiling vertex shader
    glCompileShader(vertShader);
    checkShaderError(vertShader, GL_COMPILE_STATUS);

    // compiling fragment shader
    glCompileShader(fragShader);
    checkShaderError(fragShader, GL_COMPILE_STATUS);


    // attaching vertex and fragment shaders in one program
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    // linking
    glLinkProgram(shaderProgram);
    checkShaderError(shaderProgram, GL_COMPILE_STATUS);

    // detach the vertex and fragment shader
    glDetachShader(shaderProgram, vertShader);
    glDetachShader(shaderProgram, fragShader);

    // delete vertex and fragment shader because
    // they are linked into a program
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return shaderProgram;
}

GLuint createTriangle() {
    GLuint VAO; // vertex buffer object
    GLuint VBO; // vertex array object
    GLuint EBO; // element buffer object

    // vertex data x0,y0,z0,r0,b0,g0,x1,y1,z1,r1,g1,b1, ...
    GLfloat triangle_vertices[9] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    // triangle indices (vertex 0, 1 and 2)
    GLuint triangle_indices[3] = {
            0, 1, 2
    };


    glGenVertexArrays(1, &VAO); // first we have to generate the VAO
    glGenBuffers(1, &VBO);      // as well as the VBO
    glGenBuffers(1, &EBO);      // and the EBO

    glBindVertexArray(VAO);    // we now bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // we bind the VBO and specify it as an GL_ARRAY_BUFFER

    // we now have to "link" the data with the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);

    // now we enable an attribute that we can use in our vertex shader
    glEnableVertexAttribArray(0);
    // we bind the VBO that contains the triangle vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // we specify how the data is structures (
    // the parameter can be looked up under: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    // unbind the VAO, EBO, VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // VBO and EAO are no longer required
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    return VAO;
}

GLuint createCube() {
    // TODO: implement
    return 0;
}

// this function uploads the model, view and projection matrix to the shader if they are defined in the shader
void uploadMatrices(GLuint shader){
    glm::vec3 Position{0.4f, 0.0f, 3.0f};
    glm::vec3 Front{0.0f, 0.0f, -1.0f};
    glm::vec3 Up{0.0f, 1.0f, 0.0f};

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view =  glm::lookAt(Position, Position + Front, Up);
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width / (float)height, 0.1f, 10000.0f);

    // TODO: implement the rotation of the cube and the triangle if the key 0 was pressed

    GLuint viewLocation = glGetUniformLocation(shader, "view");
    if (viewLocation == -1) std::cout << "view does not exist in current shader!\n";
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE,  &view[0][0]);

    // getting the location ID of uModel in the shader
    GLuint modelLocation = glGetUniformLocation(shader, "model");
    if (modelLocation == -1) std::cout << "model does not exist in current shader!\n";
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE,  &model[0][0]);

    // getting the location ID of uModel in the shader
    GLuint projectionLocation = glGetUniformLocation(shader, "projection");
    if (projectionLocation == -1) std::cout << "projection does not exist in current shader!\n";
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE,  &projection[0][0]);
}

void draw_triangle(GLuint shader, GLuint VAO) {
    // we select the shader program that will work with our data
    glUseProgram(shader);
    // upload the model view and projection matrix to the shader
    uploadMatrices(shader);
    // here we draw our triangle
    glBindVertexArray(VAO); // we first bind the VAO again
    // we now invoke the draw call, here the count parameter is the number of elements in our indices array
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *) 0);
    // and unbind the VAO
    glBindVertexArray(0);
    // we can now deselect the shader program
    glUseProgram(0);

}

void draw_cube(GLuint shader, GLuint VAO) {
    // TODO: implement
}

// this function is called when a key is pressed
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // if the escape key is pressed the window will close
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // if this key is pressed the cube should start rotating around the y-axis
    if (key == GLFW_KEY_0 && action == GLFW_PRESS) {

        // TODO: implement

    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (wireframe) {
            wireframe = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            wireframe = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
}

// this function is called when the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, const char *argv[]) {
    // initialize the GLFW library to be able create a window
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't init GLFW");
    }

    // set the opengl version
    int major = 3;
    int minor = 2;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create the window
    GLFWwindow *window = glfwCreateWindow(640, 480, "Lab 1", NULL, NULL);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a window");
    }

    // set the window to the current context so that it is used
    glfwMakeContextCurrent(window);
    // set the frameBufferSizeCallback so that the window adjusts if it is scaled
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // set the keyCallback function so that keyboard input can be used
    glfwSetKeyCallback(window, key_callback);

    // try to initialise glew to be able to use opengl commands
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error(
                std::string("Could initialize GLEW, error = ") + (const char *) glewGetErrorString(err));
    }

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;


    // opengl configuration
    glEnable(GL_DEPTH_TEST);        // enable depth-testing
    glDepthFunc(GL_LESS);           // depth-testing interprets a smaller value as "closer"
    glfwSwapInterval(false); // disables VSYNC

    GLuint triangleVAO = createTriangle();
    GLuint cubeVAO = createCube();

    GLuint triangleShader = createShader(
            "../shader/triangle.vert",
            "../shader/triangle.frag");

    // TODO: create and load new shaders for the cube

    // the main loop where the object are drawn
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height);

        glClearColor(0.63f, 0.63f, 0.63f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_triangle(triangleShader, triangleVAO);

        // TODO: draw the cube

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // unbinding shader program
    glUseProgram(NULL);
    // deleting shader program
    glDeleteProgram(triangleShader);

    // TODO: delete the cube shader

    // deleting vertex array object
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    // window is no longer required
    glfwDestroyWindow(window);
    // finish glfw
    glfwTerminate();
    return 0;
}

