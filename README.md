# cpp_learn_warehouse

#### 介绍
cpp的学习仓库

#### 学习进度

1.  进程池v1.0 搭建进程池整体框架
    问题：
        传递文件描述符时，服务端struct msghdr的正文部分应该有内容，cmsghdr记录一次长度之后，msg也要记录一次长度；
        客户端接收文件描述符时，应提前创建好struct iovec和struct cmsghdr所用空间。
2.  进程池v2.0 解决应用层数据TCP传输无边界，粘包和半包问题，用户途中关闭连接
3.  进程池v2.1 mmap和sendfile函数实现零拷贝，手动实现MSG_WAITALL效果（recvn函数）

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
