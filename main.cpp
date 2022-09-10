#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "loadFrame.h"
#include<iostream>
#include "Shader.h"
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//bool load_frame(const char* filename, int* width, int* height, unsigned char** data);
int main()
{
    GLFWwindow* window;
    if(!glfwInit()){
        return 1;
    }

    window = glfwCreateWindow(640,480,"Hello",NULL,NULL);
    if(!window){
        return 1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glewExperimental=GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

    }
//        int frame_width,frame_height;
//        unsigned char* frame_data;
//        if(!load_frame("output2.mkv",&frame_width,&frame_height,&frame_data)){
//            printf("Couldnt load video frame!\n");
//            return 1;
//        }


    Shader ourShader("vertex.glsl", "fragment.glsl");

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);




    //    unsigned char* data = new unsigned char[100*100*3];
    //    for(int y=0; y<100;y++){
    //        for(int x = 0;x<100;++x){
    //            data[y*100*3+x*3]=0xff;
    //            data[y*100*3+x*3+1]=0xff;
    //            data[y*100*3+x*3+2]=0xff;
    //        }
    //    }
    //    for(int y=25; y<75;y++){
    //        for(int x = 25;x<75;++x){
    //            data[y*100*3+x*3]=0xff;
    //            data[y*100*3+x*3+1]=0x00;
    //            data[y*100*3+x*3+2]=0xff;
    //        }
    //    }
    VideoState vstate;
    if(!video_open(&vstate,"output2.mkv")){
        return 1;
    }
    constexpr int ALIGNMENT = 128;
    const int frame_width = vstate.width;
    const int frame_height = vstate.height;
    uint8_t* frame_data;
    if (posix_memalign((void**)&frame_data, ALIGNMENT, frame_width * frame_height * 4) != 0) {
        printf("Couldn't allocate frame buffer\n");
        return 1;
    }



    unsigned int tex_handle;
    //    int tex_width =100;
    //    int tex_height = 100;

    glGenTextures(1,&tex_handle);
    glBindTexture(GL_TEXTURE_2D,tex_handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,frame_width,frame_height,0,GL_RGBA,GL_UNSIGNED_BYTE,frame_data);
    glGenerateMipmap(GL_TEXTURE_2D);



    int64_t pts;

    ourShader.use();
    ourShader.setInt("texture1", 0);




    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


        if (!video_readframe(&vstate, frame_data,&pts)) {
            printf("Couldn't load video frame111111111111\n");
            return 1;
        }

        static bool first_frame = true;
        if (first_frame) {
            glfwSetTime(0.0);
            first_frame = false;
        }

        double pt_in_seconds = pts * (double)vstate.time_base.num / (double)vstate.time_base.den;
        while (pt_in_seconds > glfwGetTime()) {
            glfwWaitEventsTimeout(pt_in_seconds - glfwGetTime());
        }




 glBindTexture(GL_TEXTURE_2D,tex_handle);
 glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,frame_width,frame_height,0,GL_RGBA,GL_UNSIGNED_BYTE,frame_data);
 glGenerateMipmap(GL_TEXTURE_2D);

 glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
         transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
         transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

         // get matrix's uniform location and set matrix
          glActiveTexture(GL_TEXTURE0);

         unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
         glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));








        // render container
        ourShader.use();


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    video_close(&vstate);
    std::cout << "Hello World!" << std::endl;
    return 0;
}
