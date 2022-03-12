int main(){
	// 初始化glfw与glad相关参数、创建上下文、创建窗口
	initAll();

	// 注册鼠标、滚轮、键盘回调函数
	registerCallbackFunction();

	// 初始化游戏参数
	myGame.init();

	// 渲染主循环,当收到窗口关闭信号的时候终止循环
	while(!glfwWindowShouldClose(window))
	{
		// 计算前一帧与当前帧相隔时间
		dt = currentFrame - lastFrame;

		// 响应键盘鼠标输入
		myGame.ProcessInput();

		// 如果还没撞机坠毁
		if(！myGame.isCrash){
			//更新游戏参数
			myGame.Update(dt);
			//渲染图像
			myGame.Render();
		}
		// 如果已经撞机坠毁
		else{
			myGame.UpdateAfterCrash(dt);
			myGame.RenderAfterCrash();
		}

		// 交换缓冲区
		glfwSwapBuffers(window);
	}
}


