# Pyrite

Pyrite 是一个基于 UDP 的异步通讯协议，支持为数据包分配路由函数，也支持等待回信。

## 安装

Linux

```shell
sudo make install
# 如果需要
make clean
```

## 使用

**一个简单的示例**

server.cc

```cc

prt::bytes recv_print(sockaddr_in addr, prt::bytes data) {
  return prt::bytes("server received: " + data.to_string());
}

int main() {
  prt::server s(8080);
  s.set_handler("print", recv_print);
  s.start();
  return 0;
}

```

client.cc

```cc

int main() {
  prt::client c("127.0.0.1", 8080);
  c.async();
  std::string input;
  while (true) {
    std::cin >> input;
    std::cout << c.promise("print", prt::bytes(input)).to_string() << std::endl;
  }
  return 0;
}

```
