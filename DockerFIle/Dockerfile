FROM ubuntu:20.04
ARG arch_name=amd64

# 前几步因为针对本实验依赖不全的原因问题百出，所以放弃了，还是ubuntu20.04官方镜像源对于MIT6.S081所需依赖最全面
# 1.备份源列表
# RUN cp /etc/apt/sources.list /etc/apt/sources.backup.list
# 2.把本目录下的sources.list中的镜像源添加到Docker中，下载速度起飞
# COPY sources.list /etc/apt/sources.list
# 3.更新源
# RUN apt-get update 

# 创建一个mit6s081的用户和其home目录
RUN useradd -m mit6s081 && \
    echo "root ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers && \
    echo "mit6s081 ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers 
    
# 设置一些环境变量
ENV TZ=Asia/Shanghai \
    LANG=en_US.utf8 \
    LANGUAGE=en_US.UTF-8 \
    LC_ALL=en_US.UTF-8 \
    DEBIAN_FRONTEND=noninteractive


# MIT6.S081 Lab所用依赖
# 1.安装RISC-V交叉编译工具和一些其他的常用工具
RUN apt-get update && \
    apt-get install -y sudo dos2unix git wget vim build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu libpixman-1-dev gcc-riscv64-unknown-elf libglib2.0-dev pkg-config
# 2.安装QEMU和配置QEMU
RUN wget https://download.qemu.org/qemu-5.1.0.tar.xz 
RUN tar xf qemu-5.1.0.tar.xz 
RUN cd qemu-5.1.0 && \
    ./configure --disable-kvm --disable-werror --prefix=/usr/local --target-list=riscv64-softmmu && \
    make && \
    make install

# 下载code-server并安装
RUN apt-get install -y aria2 && \
    aria2c https://github.com.cnpmjs.org/cdr/code-server/releases/download/v3.12.0/code-server_3.12.0_${arch_name}.deb && \
    dpkg -i code-server_3.12.0_${arch_name}.deb

# 切换用户mit6s081
USER mit6s081
# 下载一些code-server的插件
RUN mkdir /home/mit6s081/extensions
# 1.Markdown Extension
RUN code-server --install-extension yzhang.markdown-all-in-one
# 2.Cpp Extension
ADD cpptools-linux.vsix /home/mit6s081/extensions
RUN code-server --install-extension /home/mit6s081/extensions/cpptools-linux.vsix
# 3.Material Theme Extension
RUN code-server --install-extension equinusocio.vsc-material-theme 

# 切换回Root用户，拥有最高权限
USER root
RUN apt-get update

# 暴露8848端口，用于code-server本地运行的端口
EXPOSE 8848
# 设置code-server密码
ENV PASSWORD=mit6s081

USER mit6s081
CMD [ "code-server", "--bind-addr", "0.0.0.0:8848", "--auth", "password" ]
