/*
ZJ Wood CPE 471 Lab 3 base code
*/

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

#include "WindowManager.h"
#include "Shape.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;
double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime =glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}
class camera
{
public:
	glm::vec3 pos, rot;
	int w, a, s, d;
	camera()
	{
		w = a = s = d = 0;
		pos = glm::vec3(0, 0, -15);
		rot = glm::vec3(0, 0, 0);
	}
	glm::mat4 process(double frametime)
	{
		double ftime = frametime;
		float speed = 0;
		if (w == 1)
		{
			speed = 10*ftime;
		}
		else if (s == 1)
		{
			speed = -10*ftime;
		}
		float yangle=0;
		if (a == 1)
			yangle = -1*ftime;
		else if(d==1)
			yangle = 1*ftime;
		rot.y += yangle;
		glm::mat4 R = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
		glm::vec4 dir = glm::vec4(0, 0, speed,1);
		dir = dir*R;
		pos += glm::vec3(dir.x, dir.y, dir.z);
		glm::mat4 T = glm::translate(glm::mat4(1), pos);
		return R*T;
	}
};

camera mycam;

bool left_r, right_r;

class Application : public EventCallbacks
{

public:
	int kn = 0;
	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog, shapeprog, prog2, globeprog, progT, progE, progC, progN, progM;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID, VAO, VAO2;

	// Data necessary to give our box to OpenGL
	GLuint VertexBufferID, VertexColorIDBox, IndexBufferIDBox, VBO1, VBO2, VBO3, V2BO1, V2BO2;

	Shape shape;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			mycam.w = 1;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			mycam.w = 0;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			mycam.s = 1;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			mycam.s = 0;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
            left_r = 1;
			//mycam.a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
            left_r = 0;
			//mycam.a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
            right_r = 1;
			//mycam.d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
            right_r = 0;
			//mycam.d = 0;
		}
		if (key == GLFW_KEY_N && action == GLFW_PRESS) kn = 1;
		if (key == GLFW_KEY_N && action == GLFW_RELEASE) kn = 0;
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;
	//	float newPt[2];
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "Pos X " << posX <<  " Pos Y " << posY << std::endl;

		}
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

    int numTri = 100;
    
    vector<GLfloat> makeCircle(float r, float z)
    {
        vector<GLfloat> new_vertex;
        float twoPi = 2.0 * 3.141592654 + 1;
         
        int a = 1; //circle index

        for (int x = 0; x < numTri; x++) {
            //loop through one triangle
           // for (int y = 0; y < 2; y++) {
                    new_vertex.push_back(r*cos(a * twoPi / numTri));
                    new_vertex.push_back(r*sin(a * twoPi / numTri));
                    new_vertex.push_back(z);
               // }
          //  }
            a++;
        }
        return new_vertex;
    }
    
    vector<GLfloat> makeCylinder(vector<GLfloat>&cyl_vertex, vector<GLfloat>&cyl_vertex2) {
        
        vector<GLfloat> c_vertex;
        
        for (int i  = 0; i <cyl_vertex.size()/3 ; i++)
        {
            float x1, x2, x3, y1, y2, y3, z1, z2, z3, x4, y4, z4;
            
            x1 = cyl_vertex[(3 * i)];
            y1 = cyl_vertex[(3 * i) + 1];
            z1 = cyl_vertex[(3 * i) + 2];
               
            x3 = cyl_vertex2[(3 * i)];
            y3 = cyl_vertex2[(3 * i) + 1];
            z3 = cyl_vertex2[(3 * i) + 2];
               
            i++;
        
            x2 = cyl_vertex[(3 * i)];
            y2 = cyl_vertex[(3 * i) + 1];
            z2 = cyl_vertex[(3 * i) + 2];
               
            x4 = cyl_vertex2[(3 * i)];
            y4 = cyl_vertex2[(3 * i) + 1];
            z4 = cyl_vertex2[(3 * i) + 2];
                   
            i--;
                   
            //first tri, first vert
            c_vertex.push_back(x1);
            c_vertex.push_back(y1);
            c_vertex.push_back(z1);
            //first tri, second vert
            c_vertex.push_back(x2);
            c_vertex.push_back(y2);
            c_vertex.push_back(z2);
            //first tri, third vert
            c_vertex.push_back(x3);
            c_vertex.push_back(y3);
            c_vertex.push_back(z3);
                   
            //second tri, first vert
            c_vertex.push_back(x3);
            c_vertex.push_back(y3);
            c_vertex.push_back(z3);
            //second tri, second vert
            c_vertex.push_back(x2);
            c_vertex.push_back(y2);
            c_vertex.push_back(z2);
            //third tri, third vert
            c_vertex.push_back(x4);
            c_vertex.push_back(y4);
            c_vertex.push_back(z4);
        }
        
        return c_vertex;
    }
    
    
	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
		string resourceDirectory = "../../resources";
		//try t800.obj or F18.obj ...
		shape.loadMesh(resourceDirectory + "/sphere.obj");
		shape.resize();
		shape.init();


        //*************************************
		//generate the VAO
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		//generate vertex buffer to hand off to OGL
		glGenBuffers(1, &VertexBufferID);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

		GLfloat cube_vertices[] = {
			// front
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			// back
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
			//tube 8 - 11
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			//12 - 15
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0

			
		};
		//actually memcopy the data - only do this once
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

		//we need to set up the vertex array
		glEnableVertexAttribArray(0);
		//key function to get up how many elements to pull out at a time (3)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		//color
		GLfloat cube_colors[] = {
			// front colors
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			// back colors
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			// tube colors
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
			0.9, 0.9, 0.0,
		};
		glGenBuffers(1, &VertexColorIDBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VertexColorIDBox);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &IndexBufferIDBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		GLushort cube_elements[] = {
		
			// front
			0, 1, 2,
			2, 3, 0,
			// back
			7, 6, 5,
			5, 4, 7,
			//tube 8-11, 12-15
			8,12,13,
			8,13,9,
			9,13,14,
			9,14,10,
			10,14,15,
			10,15,11,
			11,15,12,
			11,12,8
			
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

		glBindVertexArray(0);
        
        //cylinder
        //*************************************
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
         

        //vertex buffer, positions
        glGenBuffers(1, &VBO1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
         
        vector<GLfloat> cyl_vertex, cyl_vertex2;
        
        cyl_vertex = makeCircle(1, 0.0);
        cyl_vertex2 = makeCircle(1, -1.0);
        
        vector<GLfloat> c_vertex;
     
        c_vertex = makeCylinder(cyl_vertex, cyl_vertex2);

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * c_vertex.size(), c_vertex.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

         //fragment buffer, color
        glGenBuffers(1, &VBO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
         
        vector<GLfloat> cyl_color;
        
        for (int i = 0; i < c_vertex.size(); i++) {
            cyl_color.push_back(1);
            cyl_color.push_back(1);
            cyl_color.push_back(0.0);
        }
         
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * cyl_color.size(), cyl_color.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        
        //normals
        glGenBuffers(1, &VBO3);
        glBindBuffer(GL_ARRAY_BUFFER, VBO3);
         
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * c_vertex.size(), c_vertex.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        
        glBindVertexArray(0);
//****************************************************************************************

	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
		// Enable blending/transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Initialize cube
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
		if (!prog->init())
			{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1); //make a breakpoint here and check the output window for the error message!
			}
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertColor");
        prog->addAttribute("vertNorm");
        
        //initialize tail
        progT = std::make_shared<Program>();
        progT->setVerbose(true);
        progT->setShaderNames(resourceDirectory + "/tail_vertex.glsl", resourceDirectory + "/tail_fragment.glsl");
        if (!progT->init())
            {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1); //make a breakpoint here and check the output window for the error message!
            }
        progT->addUniform("P");
        progT->addUniform("V");
        progT->addUniform("M");
        progT->addAttribute("vertPos");
        progT->addAttribute("vertColor");
        progT->addAttribute("vertNorm");
        
        //initialize nose
        progN = std::make_shared<Program>();
        progN->setVerbose(true);
        progN->setShaderNames(resourceDirectory + "/nose_vertex.glsl", resourceDirectory + "/nose_fragment.glsl");
        if (!progN->init())
            {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1); //make a breakpoint here and check the output window for the error message!
            }
        progN->addUniform("P");
        progN->addUniform("V");
        progN->addUniform("M");
        progN->addAttribute("vertPos");
        progN->addAttribute("vertColor");
        progN->addAttribute("vertNorm");
        
		// Initialize sphere
		shapeprog = std::make_shared<Program>();
		shapeprog->setVerbose(true);
		shapeprog->setShaderNames(resourceDirectory + "/shape_vertex.glsl", resourceDirectory + "/shape_fragment.glsl");
		if (!shapeprog->init())
			{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1); //make a breakpoint here and check the output window for the error message!
			}
		shapeprog->addUniform("P");
		shapeprog->addUniform("V");
		shapeprog->addUniform("M");
        shapeprog->addUniform("camPos");
		shapeprog->addAttribute("vertPos");
		shapeprog->addAttribute("vertNor");
		shapeprog->addAttribute("vertTex");
        shapeprog->addAttribute("vecPos");
        
        //initialize eyes
        progE = std::make_shared<Program>();
        progE->setVerbose(true);
        progE->setShaderNames(resourceDirectory + "/eye_vertex.glsl", resourceDirectory + "/eye_fragment.glsl");
        if (!progE->init())
            {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1); //make a breakpoint here and check the output window for the error message!
            }
        progE->addUniform("P");
        progE->addUniform("V");
        progE->addUniform("M");
        progE->addUniform("camPos");
        progE->addAttribute("vertPos");
        progE->addAttribute("vertNor");
        progE->addAttribute("vertTex");
        progE->addAttribute("vecPos");
        
        //initialize cheeks
        progC = std::make_shared<Program>();
        progC->setVerbose(true);
        progC->setShaderNames(resourceDirectory + "/cheek_vertex.glsl", resourceDirectory + "/cheek_fragment.glsl");
        if (!progC->init())
            {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1); //make a breakpoint here and check the output window for the error message!
            }
        progC->addUniform("P");
        progC->addUniform("V");
        progC->addUniform("M");
        progC->addAttribute("vertPos");
        progC->addAttribute("vertNor");
        progC->addAttribute("vertTex");
        progC->addAttribute("vecPos");
        
        //initialize mouth
        progM = std::make_shared<Program>();
        progM->setVerbose(true);
        progM->setShaderNames(resourceDirectory + "/mouth_vertex.glsl", resourceDirectory + "/mouth_fragment.glsl");
        if (!progM->init())
            {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1); //make a breakpoint here and check the output window for the error message!
            }
        progM->addUniform("P");
        progM->addUniform("V");
        progM->addUniform("M");
        progM->addAttribute("vertPos");
        progM->addAttribute("vertNor");
        progM->addAttribute("vertTex");
        progM->addAttribute("vecPos");
        
        
        //initialize cylinder
        prog2 = std::make_shared<Program>();
        prog2->setVerbose(true);
        prog2->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
        prog2->init();
        
        prog2->addUniform("P");
        prog2->addUniform("V");
        prog2->addUniform("M");
        prog2->addAttribute("vertPos");
        prog2->addAttribute("vertColor");
        prog2->addAttribute("vertNorm");
        
        //initialize snowglobe
        globeprog = std::make_shared<Program>();
        globeprog->setVerbose(true);
        globeprog->setShaderNames(resourceDirectory + "/globe_vertex.glsl", resourceDirectory + "/globe_fragment.glsl");
        if (!globeprog->init())
            {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1); //make a breakpoint here and check the output window for the error message!
            }
        globeprog->addUniform("P");
        globeprog->addUniform("V");
        globeprog->addUniform("M");
        globeprog->addAttribute("vertPos");
        globeprog->addAttribute("vertNor");
        globeprog->addAttribute("vertTex");
        globeprog->addAttribute("vecPos");
        
	}


	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{

		double frametime = get_last_elapsed_time();
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	//	float aspect = width/(float)height;
		glViewport(0, 0, width, height);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		// Create the matrix stacks - please leave these alone for now
		
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = glm::mat4(1);
		M = glm::mat4(1);
		// Apply orthographic projection....
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones

//glm::mat4 TransX = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 0.0f, 0.0));
		//glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        
        //declaring variables
        mat4 R, T, Mhead, Mbody, Mland, Mrand, Mlleg, Mrleg, Mltoe, Mrtoe, Mlarm, Mrarm, Mlfo, Mrfo, Mlear, Mrear, M1, M2, M3, M4, M5, M6, Mle, Mre, Mlc, Mrc, Mm, Mn;
//left_r, right_r;
        static float rot = 0.0;
        if (left_r == 1)
        {
            rot += 0.05;
        }
        if (right_r == 1)
        {
            rot -= 0.05;
        }

        //body
		mat4 Sbody = glm::scale(glm::mat4(1.0f), glm::vec3(1.1, 1.2, 1));
        mat4 Tbody = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.7, 0));
        mat4 Rot = rotate(mat4(1.0f), rot, vec3(0, 1, 0));
        Mbody = Tbody * Rot;
       // M = Mbody * Sbody;
        
        //Cubes
        progT->bind();

		V = mycam.process(frametime);
		//send the matrices to the shaders
		glUniformMatrix4fv(progT->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(progT->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(progT->getUniform("M"), 1, GL_FALSE, &M[0][0]);

		glBindVertexArray(VertexArrayID);
		//actually draw from vertex 0, 3 vertices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		
        //tail part1
        static float a1 = -0.0;
        a1 += 0.05 * sin(glfwGetTime() + 2.3) / 3;
        static float aa = 0.5;
        mat4 S1 = scale(mat4(1.0f), vec3(0.7, 0.15, 0.06));
		mat4 T1 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.6, -1));
        mat4 Tr = translate(mat4(1.0f), vec3(0.7, 0, 0));
        mat4 Rr1 = rotate(mat4(1.0f), aa, vec3(-1, 1.6, 0.0));
        mat4 R1 = rotate(mat4(1.0f), a1, vec3(0.0, 0.0, -1.0));
        M1 = Mbody * Rr1 * T1 * R1 * Tr;
		M = M1 * S1;
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        progT->unbind();
		
        prog->bind();

        V = mycam.process(frametime);
        //send the matrices to the shaders
        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);

        glBindVertexArray(VertexArrayID);
        //actually draw from vertex 0, 3 vertices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
        
        //tail part2
        mat4 S2 = scale(mat4(1.0f), vec3(0.25, 0.32, 0.05));
        mat4 T2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.375, 0.3, -0.001));
        M2 = M1 * T2;
        M = M2 * S2;
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        
        //tail part3
        static float a3 = 0.5;
        mat4 S3 = scale(mat4(1.0f), vec3(0.6, 0.25, 0.05));
        mat4 T3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.39, 0.415, 0.0));
        mat4 R3 = rotate(mat4(1.0f), a3, vec3(0.0, 0.0, 1.0));
        M3 = M2 * T3 * R3;
        M = M3 * S3;
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        //tail part4
        static float a4 = 0.0;
        mat4 S4 = scale(mat4(1.0f), vec3(0.3, 0.6, 0.05));
        mat4 T4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.7, 0.35, 0.0));
        mat4 R4 = rotate(mat4(1.0f), a4, vec3(0.0, 0.0, -1.0));
        M4 = M3 * T4 * R4;
        M = M4 * S4;
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        //tail part5
        static float a5 = 1.1;
        mat4 S5 = scale(mat4(1.0f), vec3(0.5, 0.9, 0.05));
        mat4 T5 = glm::translate(glm::mat4(1.0f), glm::vec3(0.3, 0.65, 0.0));
        mat4 R5 = rotate(mat4(1.0f), a5, vec3(0.0, 0.0, -1.0));
        M5 = M4 * T5 * R5;
        M = M5 * S5;
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
		prog->unbind();
        
        progN->bind();

        V = mycam.process(frametime);
        //send the matrices to the shaders
        glUniformMatrix4fv(progN->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(progN->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(progN->getUniform("M"), 1, GL_FALSE, &M[0][0]);

        glBindVertexArray(VertexArrayID);
        //actually draw from vertex 0, 3 vertices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
        
        static float n = 0.7;
        //nose
        mat4 Sn = scale(mat4(1.0f), vec3(0.1, 0.1, 0.1));
        mat4 Tn = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 1.4, 1.07));
        mat4 Rn2 = rotate(mat4(1.0f), n, vec3(0.8, 0, 0));
        mat4 Rn = rotate(mat4(1.0f), n, vec3(0, 1, 0));
        Mn = Mbody * Tn * Rn2 * Rn;
        M = Mn * Sn;
        glUniformMatrix4fv(progN->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        progN->unbind();
        
        //spheres
		shapeprog->bind();

        //head
		mat4 Shead = glm::scale(glm::mat4(1.0f), glm::vec3(1.2, 1.2, 1.2));
		mat4 Thead = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.5, 0));
        Mhead = Mbody * Thead;
		M = Mhead * Shead;
		glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
        
        //body
       // mat4 Sbody = glm::scale(glm::mat4(1.0f), glm::vec3(1.1, 1.2, 1));
       // mat4 Tbody = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.7, 0));
        //Mbody = Tbody;
        M = Mbody * Sbody;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
        
        //left ear
        static float lE = 0.3;
        mat4 Slear = scale(mat4(1.0f), vec3(0.2, 1.0, 0.2));
        mat4 Rlear = rotate(mat4(1.0f), lE, vec3(0, 0, 1));
        mat4 Tplear = translate(mat4(1.0f), vec3(0, 1.0, 0.0));
        mat4 Tolear = translate(mat4(1.0f), vec3(-0.7, 0.8, 0.0));
        Mlear = Mhead * Tolear * Rlear * Tplear;
        M = Mlear * Slear;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
        
        //left arm
        static float cL = 1.5;
        static float W = 0.35;
        W += 0.05 * sin(5 *glfwGetTime() + 2) / 10;
        mat4 Slarm = scale(mat4(1.0f), vec3(0.2, 0.2, 1.7));
        mat4 Rlarm = rotate(mat4(1.0f), cL, vec3(1.0, 1.0, 0.0));
        mat4 Tplarm = translate(mat4(1.0f), vec3(0, 0, -1.0));
        mat4 Tolarm = translate(mat4(1.0f), vec3(0.15, -0.35, 0.1));
        mat4 Lw = rotate(mat4(1.0f), W, vec3(0, 0, 1));
        Mlarm = Lw * Mbody * Tolarm * Rlarm * Tplarm;
        
        //left hand
        mat4 Sland = scale(mat4(1.0f), vec3(0.22, 0.22, 0.22));
        mat4 Toland = translate(mat4(1.0f), vec3(0.0, 0.0, -1.7));
        Mland = Mlarm * Toland;
        M = Mland * Sland;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
        
        //right ear
        static float rE = 0.3;
        mat4 Srear = scale(mat4(1.0f), vec3(0.2, 1.0, 0.2));
        mat4 Rrear = rotate(mat4(1.0f), rE, vec3(0, 0, -1));
        mat4 Tprear = translate(mat4(1.0f), vec3(0, 1.0, 0.0));
        mat4 Torear = translate(mat4(1.0f), vec3(0.7, 0.8, 0.0));
        Mrear = Mhead * Torear * Rrear * Tprear;
        M = Mrear * Srear;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);

        //right arm
        static float cR = 1.5;
        static float w = -0.25;
        w += 0.05 * sin(5 *glfwGetTime() + 2) / 10;
        mat4 Srarm = scale(mat4(1.0f), vec3(0.2, 0.2, 1.7));
        mat4 Rrarm = rotate(mat4(1.0f), cR, vec3(1.0, -1.0, 0.0));
        mat4 Tprarm = translate(mat4(1.0f), vec3(0, 0, -1.1));
        mat4 Torarm = translate(mat4(1.0f), vec3(-0.13, -0.4, 0.1));
        mat4 Rw = rotate(mat4(1.0f), w, vec3(0, 0, 1));
        Mrarm = Rw * Mbody * Torarm * Rrarm * Tprarm;
        
        //right hand
        mat4 Srand = scale(mat4(1.0f), vec3(0.22, 0.22, 0.22));
        mat4 Trand = translate(mat4(1.0f), vec3(0.0, 0.0, -1.7));
        Mrand = Mrarm * Trand;
        M = Mrand * Srand;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
        
        //left leg
        mat4 Slleg = scale(mat4(1.0f), vec3(0.4, 0.5, 0.4));
        mat4 Tlleg = translate(mat4(1.0f), vec3(-0.6, -0.8, 0.8));
        Mlleg = Mbody * Tlleg;
        M = Mlleg * Slleg;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
        
        //right leg
        mat4 Srleg = scale(mat4(1.0f), vec3(0.5, 0.5, 0.5));
        mat4 Trleg = translate(mat4(1.0f), vec3(0.6, -1., 0.0));
        Mrleg = Mbody * Trleg;
        M = Mrleg * Srleg;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
        
        //left foot
        static float fL = 1.8;
        mat4 Slfo = scale(mat4(1.0f), vec3(0.13, 0.1, 0.45));
        mat4 Rlfo = rotate(mat4(1.0f), fL, vec3(1.0, 0.1, 0.4));
        mat4 Tlfo = translate(mat4(1.0f), vec3(0.0, -0.4, 0.4));
        Mlfo = Mlleg * Tlfo * Rlfo;
        
        //left toes
        mat4 Sltoe = scale(mat4(1.0f), vec3(0.13, 0.12, 0.1));
        mat4 Tltoe = translate(mat4(1.0f), vec3(-0.23, 0.04, 0.45));
        Mltoe = Mlleg * Tltoe;
        M = Mltoe * Sltoe;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
        
        //right foot
        static float fR = 1.5;
        mat4 Srfo = scale(mat4(1.0f), vec3(0.13, 0.12, 0.45));
        mat4 Rrfo = rotate(mat4(1.0f), fR, vec3(1.0, 0.0, 0.0));
        mat4 Trfo = translate(mat4(1.0f), vec3(0.0, -0.8, -0.2));
        Mrfo = Mrleg * Trfo * Rrfo;
        
        //right toes
        mat4 Srtoe = scale(mat4(1.0f), vec3(0.13, 0.12, 0.12));
        mat4 Trtoe = translate(mat4(1.0f), vec3(0.0, -0.8, -0.2));
        Mrtoe = Mrleg * Trtoe;
        M = Mrtoe * Srtoe;
        glUniformMatrix4fv(shapeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(shapeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(shapeprog);
		shapeprog->unbind();
        
        //cylinders
        prog2->bind();
        glBindVertexArray(VAO);
        //left arm
        M = Mlarm * Slarm;
        glUniformMatrix4fv(prog2->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(prog2->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(prog2->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, numTri * 6 - 6);
        
        //right arm
        M = Mrarm * Srarm;
        glUniformMatrix4fv(prog2->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(prog2->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(prog2->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, numTri * 6 - 6);
        
        //left foot
        M = Mlfo * Slfo;
        glUniformMatrix4fv(prog2->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(prog2->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(prog2->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, numTri * 6 - 6);
        
        //right foot
        M = Mrfo * Srfo;
        glUniformMatrix4fv(prog2->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(prog2->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(prog2->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, numTri * 6 - 6);
        
        
        glBindVertexArray(0);
        prog2->unbind();
        
        
        //eyes
        progE->bind();
        //left eye
        mat4 Sle = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.15));
        mat4 Tle = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0.15, 1.05));
        Mle = Mhead * Tle;
        M = Mle * Sle;
        glUniformMatrix4fv(progE->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(progE->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(progE->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        vec3 campos = -mycam.pos;
        glUniform3fv(progE->getUniform("camPos"), 1, &campos.x);
        shape.draw(progE);
        
        //right eye
        mat4 Sre = glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.15));
        mat4 Tre = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.15, 1.05));
        Mre = Mhead * Tre;
        M = Mre * Sre;
        glUniformMatrix4fv(progE->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(progE->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(progE->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(progE);
        
        progE->unbind();
        
        progC->bind();
        static float c = 1.0;

        //left cheek
        mat4 Slc = glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.3, 0.1));
        mat4 Tlc = glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, -0.3, 0.8));
        mat4 Rlc = rotate(mat4(1.0f), c, vec3(0, -1, -0.7));
        mat4 Rlc2 = rotate(mat4(1.0f), c, vec3(0.5, 0, 0));
        Mlc = Mhead * Tlc * Rlc2 * Rlc;
        M = Mlc * Slc;
        glUniformMatrix4fv(progC->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(progC->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(progC->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(progC);
        
        //right cheek
        mat4 Src = glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.3, 0.1));
        mat4 Trc = glm::translate(glm::mat4(1.0f), glm::vec3(0.75, -0.3, 0.8));
        mat4 Rrc = rotate(mat4(1.0f), c, vec3(0, 1, 0.7));
        mat4 Rrc2 = rotate(mat4(1.0f), c, vec3(0.5, 0, 0));
        Mrc = Mhead * Trc * Rrc2  * Rrc;
        M = Mrc * Src;
        glUniformMatrix4fv(progC->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(progC->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(progC->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(progC);
        
        progC->unbind();
        
        progM->bind();
       // static float c = 1.0;
        
        //mouth
        mat4 Sm = glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.2, 0.05));
        mat4 Tm = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -0.4, 1.1));
        //mat4 Rrc = rotate(mat4(1.0f), c, vec3(0, -1, 0));
        //mat4 Rrc2 = rotate(mat4(1.0f), c, vec3(-1, 0, 0));
        Mm = Mhead * Tm;
        M = Mm * Sm;
        glUniformMatrix4fv(progM->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(progM->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(progM->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(progM);
        
        progM->unbind();
        
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        globeprog->bind();

        //globe
        mat4 Sg = glm::scale(glm::mat4(1.0f), glm::vec3(4.2, 4.2, 4.2));
        mat4 Tg = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.5, 0));
        mat4 Rg = rotate(mat4(1.0f), rot, vec3(0, 1, 0));
        M = Tg * Rg * Sg;
        glUniformMatrix4fv(globeprog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(globeprog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(globeprog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        shape.draw(globeprog);
    
        globeprog->unbind();
        
        

	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
