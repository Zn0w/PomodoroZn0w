#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdlib.h>
#include <stdio.h>


static int screen_width, screen_height;

static int pomodoro_duration = 25;
static int break_duration = 5;
static int long_break_duration = 10;
static int loop = 4;	// the amount of pomodoros, after which a long break is due

static int minutes_left = pomodoro_duration;
static int seconds_left = 0;
static bool paused = true;
static bool working = true;

static int pomodoros = 0;


static void reset_timer(bool go_next)
{
	// play OS's notification sound
	printf("\a");

	if (go_next)
		working = !working;
	
	if (working)
		minutes_left = pomodoro_duration;
	else
	{
		if (go_next)
			pomodoros++;
		
		if ((pomodoros % loop) == 0)
			minutes_left = long_break_duration;
		else
			minutes_left = break_duration;
	}

	seconds_left = 0;
	paused = false;
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void init_gui(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

static void destroy_gui()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

static void construct_timer_window()
{
	ImGui::Begin("Pomodoro Timer");

	if (working)
		ImGui::Text("Focused Work");
	else
		ImGui::Text("Take a break");

	ImGui::Text("%.2d:%.2d", minutes_left, seconds_left);

	if (ImGui::Button("Start/Pause"))
	{
		// if the button has been pressed
		paused = !paused;
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		reset_timer(false);
	}
	ImGui::SameLine();
	if (ImGui::Button("Skip"))
	{
		reset_timer(true);
	}

	ImGui::Text("Pomodoros done this session: %d", pomodoros);

	ImGui::End();
}

static void construct_settings_window()
{
	ImGui::Begin("Timer Settings");

	ImGui::Text("Hello World!");

	ImGui::End();
}

static void render_gui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool show_fps = false;

	construct_timer_window();

	construct_settings_window();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit())
		exit(EXIT_FAILURE);
	
	// get screen resolution
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (mode)
	{
		screen_width = mode->width / 2;
		screen_height = mode->height / 2;
	}
	else
	{
		screen_width = 640;
		screen_height = 480;
	}

	window = glfwCreateWindow(screen_width, screen_height, "PomodoroZn0w", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to init GLAD\n");
		return false;
	}

	init_gui(window);

	double last_time = glfwGetTime();
	double current_time;

	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, screen_width, screen_height);
		glClear(GL_COLOR_BUFFER_BIT);
		
		current_time = glfwGetTime();
		if (current_time - last_time >= 1.0 && !paused)
		{
			last_time = current_time;

			if (seconds_left == 0)
			{
				if (minutes_left == 0)
				{
					reset_timer(true);
					// TODO : notify user
				}

				minutes_left--;
				seconds_left = 59;
			}
			else
				seconds_left--;
		}

		render_gui();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroy_gui();

	glfwDestroyWindow(window);
	glfwTerminate();
}