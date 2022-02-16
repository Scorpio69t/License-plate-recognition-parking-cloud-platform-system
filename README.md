# Intelligent-parking-cloud-platform-system
基于c和c++、linux、QT、opencv以及easypr(车牌识别)两大开源框架库来实现的智慧停车云平台系统。

由客户端和服务端两部分组成

服务器主要分前置服务器和后置服务器，分两个进程，主要为了后面服务器拓展，提高服务器的性能。前置服务器主要作用负责与客户端进行交互，以及将客户端请求发到后置服务器的交互，后置服务器主要作用是负责根据请求查询数据库，将结果发回到前置服务器。

# 客户端环境配置问题：

1、OpenCV3.4.2：使用cmake里面cmake-gui工具来bulid，configure之后要勾选BUILD_opencv_world（为了生成动态库）

2、EasyPR v1.6：相关下载和环境配置可以查阅网上教程

3、sqlite3：同上

4、因为是在linux下运行的，所以要安装QT在linux下

# 服务端环境配置问题：

1、建议在windows下使用Visual Studio远程linux生成代码

2、在Visual Studio下修改自己远程linux的IP，工具->选项->跨平台->连接管理器->添加自己的linux

![image](https://user-images.githubusercontent.com/94782056/154187916-d78f5427-cd85-4e09-9a8f-201fbd612475.png)

3、在项目->属性可以修改自己在linux远程生成代码的路径，我这里是在/root/project底下
![image](https://user-images.githubusercontent.com/94782056/154188108-f8aec995-8ede-452b-8fa5-a3a51d52768f.png)

4、因为用到线程，所以要在链接器->命令行里面加上-pthread
![image](https://user-images.githubusercontent.com/94782056/154188851-25215f90-7498-4b80-9c8e-8aee1bc77b67.png)

4、在后置服务器里面，要用到数据库sqlite3，所以要加上依赖库sqlite3
![image](https://user-images.githubusercontent.com/94782056/154188607-0cd80849-dafb-4c68-a3e0-a2370ba95d4c.png)

如果不能不运行的话，可以发邮箱联系本人779295285@qq.com

