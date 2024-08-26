#include <cstage.h>

typedef struct
{
    int net_fd;
    int alive;
    int index;
}Client;

typedef struct node_s
{
    Client c;
    struct node_s *next;
}Node;

typedef struct
{
    Node *p[10];
}Queue;

int head_insert(Queue *q, Client c)
{
    Node *new_node = (Node*)calloc(1, sizeof(Node));
    if(new_node == NULL)
    {
        printf("error:calloc failed in head_insert.\n");
        return -1;
    }
    new_node->c = c;
    new_node->next = q->p[c.index];
    q->p[c.index] = new_node;
    return 1;
}

int delete_node(Queue *q, int index, Client c)
{
    Node *prev = NULL;
    Node *curr = q->p[index];
    if(curr->c.net_fd == c.net_fd)
    {
        q->p[index] = curr->next;
        printf("curr net_fd = %d\n", curr->c.net_fd);
        free(curr);
        return 1;
    }
    while(curr != NULL)
    {
        if(curr->c.net_fd == c.net_fd)
        {
            prev->next = curr->next;
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}

void print_list(Node *n)
{
    while(n != NULL)
    {
        printf("netfd = %d, index = %d\n", n->c.net_fd, n->c.index);
        n = n->next;
    }
}

void print_queue(Queue q)
{
    for(int i =0; i < 10; i++)
    {
        printf("queue index = %d\n", i);
        print_list(q.p[i]);
    }
}

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socket_fd, -1, "socket");
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(argv[2]));
    sockaddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(socket_fd, 10);
    ERROR_CHECK(ret, -1, "listen");

    Queue q;
    memset(&q, 0, sizeof(Queue));
    //主set
    fd_set base_set;
    FD_ZERO(&base_set);
    FD_SET(socket_fd, &base_set);
    while(1)
    {
        //本次循环set
        fd_set set;
        memcpy(&set, &base_set, sizeof(base_set));
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        int ret = select(13, &set, NULL, NULL, &tv);
        ERROR_CHECK(ret, -1, "select");
        time_t curr_time = time(NULL);
        int insert_index = curr_time % 10;
        int notime_index = (curr_time + 1) % 10;
        if(FD_ISSET(socket_fd, &set))
        {
            int net_fd = accept(socket_fd, NULL, NULL);
            ERROR_CHECK(net_fd, -1, "accept");
            Client client_tmp;
            client_tmp.index = insert_index;
            client_tmp.net_fd = net_fd; 
            client_tmp.alive =1;
            ret = head_insert(&q, client_tmp);    
            ERROR_CHECK(ret, -1, "head_insert");
            FD_SET(net_fd, &base_set);
            printf("one client connected!\n");
            print_queue(q);
        }
        int i = (curr_time + 2) % 10;
        while(i != insert_index)
        {
            Node *curr = q.p[i];
            while(curr != NULL)
            {
                if(curr->c.alive == 1 && FD_ISSET(curr->c.net_fd, &set))
                {
                    Node *tmp = curr;
                    //搜索到某个node结点
                    char buf[60] = {0};
                    ssize_t rret = recv(curr->c.net_fd, buf, sizeof(buf), 0);
                    ERROR_CHECK(rret, -1, "recv");
                    printf("recive success!\n");
                    if(rret == 0)
                    {
                        printf("one client quit!\n");
                        FD_CLR(tmp->c.net_fd, &base_set);
                        tmp->c.alive = 0;
                        close(tmp->c.net_fd);
                        ret = delete_node(&q, i, tmp->c);
                        ERROR_CHECK(ret, -1, "delete_node");
                        printf("detele success!\n");
                        print_queue(q);
                        continue;
                    }
                    //转发
                    for(int j = 0; j < 10; j++)
                    {
                        Node *send_tmp = q.p[j];
                        while(send_tmp != NULL)
                        {
                            if(send_tmp->c.net_fd != tmp->c.net_fd && send_tmp->c.alive == 1)
                            {
                                rret = send(send_tmp->c.net_fd, buf, sizeof(buf), 0);
                                ERROR_CHECK(rret, -1, "send");
                                printf("send to index %d, netfd %d, success!\n", send_tmp->c.index, send_tmp->c.net_fd);
                            }
                            send_tmp = send_tmp->next;
                        }
                    }
                    //更新位置
                    tmp->c.index = insert_index;
                    FD_CLR(tmp->c.index, &set);
                    ret = head_insert(&q, tmp->c);
                    ERROR_CHECK(ret, -1, "head_insert");
                    ret = delete_node(&q, i, tmp->c);
                    ERROR_CHECK(ret, -1, "delete_node");
                    printf("update!\n");
                    print_queue(q);
                }
                curr = curr->next;
            }
            i = (i + 1) % 10;
        }
        /* print_queue(q); */
        //超时处理
        int clear_index = notime_index;
        /* printf("%d\n", clear_index); */
        Node *clear_tmp = q.p[clear_index];
        q.p[clear_index] = NULL;
        while(clear_tmp != NULL)
        {
            Node *save = clear_tmp->next;
            FD_CLR(clear_tmp->c.net_fd, &base_set);
            close(clear_tmp->c.net_fd);
            free(clear_tmp);
            clear_tmp = save;
        }
    }
    close(socket_fd);
}
