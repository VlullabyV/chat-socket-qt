# chat

课程作业：基于socket编程实现单播和组播的聊天程序（qt界面）。

* 服务器:server目录，维护在线用户和转发单播/多播消息；
* 客户端:chat目录，服务器的连接与断开，单播/多播消息的发送/接收，加入/退出群组；

* 必要设置：
服务器IP地址：在chat目录的chat.cpp文件中，有宏定义SERVERIP，代表服务器的IP地址，默认为127.0.0.1，实际运行之前根据服务器所在的IP地址修改。

* 可选设置：
在chat目录的chat.cpp文件和server目录的server.cpp文件中有两个相同的宏定义SERVERPORT和MULTIPORT，分别代表服务器通信的端口号和多播使用的端口号，默认分别为8888和9999。
SERVERPORT可随意修改，但必须保证两个文件中的SERVERPORT值一样。
MULTIPORT同样可以随意修改，但保证两个MULTIPORT值一样且与SERVERPORT值不一样；也可以根据实际需要修改代码改由用户自定义多播端口号。
