#include <../header/client.h>
client::client(const char *Ip, int port, int timeout)
{
    WSADATA data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        std::cout << "WSAStartup error";
    }

    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(port);
    addrServer.sin_addr.S_un.S_addr = inet_addr(Ip);

    SOCKET socketClient = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketClient == INVALID_SOCKET)
    {
        std::cout << "socket create error";
    }

    this->connection = socketClient;
    this->server = addrServer;
    this->sequence = 0;
    this->timeout = timeout;
}

bool client::AddRouter(std::string identifier, std::function<std::string(std::string)> controller)
{
    if (identifier.find("prt-") != 0)
    {
        return false;
    }

    this->router[identifier] = controller;
    return true;
}

int client::getSequence()
{
    this->sequence += 1;
    return this->sequence - 1;
}

client::~client()
{
    closesocket(this->connection);
    WSACleanup();
}

std::string client::Promise(std::string identifier, std::string body)
{
    PrtPackage req(this->session, identifier, this->getSequence(), body);
    std::string str_req = req.ToBytes();
    const char *sendingMessage = (char *)str_req.data();
    if (sizeof(sendingMessage) > MAX_TRANSMIT_SIZE)
    {
        std::cout << "content overflowed" << std::endl;
        return "";
    }

    send(this->connection, sendingMessage, sizeof(sendingMessage), 0);
    this->promiseBuffer[req.sequence] = req;
    int time = this->timeout;
    std::mutex ch;
    bool lock = 0;
    std::thread time([&](int time)
                     {
        Sleep(time);
        ch.lock();
        lock=1;
        ch.unlock(); });
    bool getlock;
    std::string respbody;
    while (1)
    {
        ch.lock();
        getlock = lock;
        ch.unlock();
        this->bufferL[req.sequence]->lock();
        if (this->promiseBuffer[req.sequence].Identifier == "prt-ack")
        {
            respbody = this->promiseBuffer[req.sequence].Body;
            this->promiseBuffer.erase(req.sequence);
            this->bufferL[req.sequence]->unlock();
            delete(this->bufferL[req.sequence]);
            this->bufferL.erase(req.sequence);
            break;
        }
        
            this->bufferL[req.sequence]->unlock();
        if (getlock)
        {
            delete(this->bufferL[req.sequence]);
            this->bufferL.erase(req.sequence);
            std::cout << "pyrite counterpart timeouted" << std::endl;
            return;
        }
    }

    return respbody;
}

int client::Tell(std::string identifier, std::string body)
{
    PrtPackage P(this->session, identifier, -1, body);
    std::string strP = P.ToBytes();
    const char *sendingMessage = (char *)strP.data();
    if (sizeof(sendingMessage) > MAX_TRANSMIT_SIZE)
    {
        std::cout << "content overflowed" << std::endl;
        return -1;
    }

    send(this->connection, sendingMessage, sizeof(sendingMessage), 0);
}

void client::processAck(PrtPackage p)
{
    if (this->promiseBuffer.find(p.sequence) == this->promiseBuffer.end())
    {
        return;
    }
    this->bufferL[p.sequence]->lock();
    this->promiseBuffer[p.sequence] = p;
    this->bufferL[p.sequence]->unlock();
}

void client::process(std::string raw)
{
    PrtPackage req(raw);
    if (req.Identifier == "invalid")
    {
        return;
    }

    this->session = req.Session;

    if (req.Identifier == "prt-ack")
    {
        this->processAck(req);
    }

    if (this->router.find(req.Identifier) == this->router.end())
    {
        return;
    }

    std::function<std::string(std::string)> f = this->router[req.Identifier];
    std::string resp = f(req.Body);
    const char *sendingMessage = (char *)resp.data();
    send(this->connection, sendingMessage, sizeof(sendingMessage), 0);
}

int client::Start()
{
    if (connect(this->connection, (struct sockaddr *)&this->server,
                sizeof(this->server)) == INVALID_SOCKET)
    {
        std::cout << "connect error";
        system("pause");
        return 1;
    }

    while (1)
    {
        char recvBuff[MAX_TRANSMIT_SIZE];
        memset(recvBuff, 0, sizeof(recvBuff));

        if (recv(this->connection, recvBuff, sizeof(recvBuff), 0) <= 0)
        {
            std::cout << "recv error";
            break;
        }
        std::string raw(recvBuff);
        std::thread p(client::process, this, raw);
        p.detach();
    }
}