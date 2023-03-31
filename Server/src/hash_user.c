#include "../inc/server.h"

t_client_info *hash_table[TABLE_SIZE];

int hash(int id)
{
    return id % TABLE_SIZE;
}

int generate_id()
{
    // Используем текущее время как seed для генератора случайных чисел
    srand(time(NULL));
    // Генерируем случайное число в диапазоне от 1 до 100000
    int id = rand() % 100000 + 1;
    return id;
}

void add_client(t_client_info *client)
{
    t_client_info *new_client;
    t_client_info *current_client;
    t_user_info *user;
    int index;

    index = hash(client->user->id);

    user = (t_user_info *)malloc(sizeof(t_user_info));
    user->id = client->user->id;
    user->username = client->user->username;
    user->password = client->user->password;

    new_client = (t_client_info *)malloc(sizeof(t_client_info));
    new_client->hash_id = index;
    new_client->socket_info = client->socket_info;
    new_client->ssl = client->ssl;
    new_client->user = user;
    new_client->next = NULL;

    // Поиск последнего элемента списка
    current_client = hash_table[index];
    if (current_client == NULL)
    {
        hash_table[index] = new_client;
    }
    else
    {
        while (current_client->next != NULL)
        {
            current_client = current_client->next;
        }
        current_client->next = new_client;
    }
}

// Функция для поиска клиента в хеш-таблице
t_client_info *find_client(int id)
{
    int index = hash(id);
    t_client_info *current_client = hash_table[index];
    while (current_client != NULL)
    {
        if (current_client->user->id == id)
        {
            return current_client;
        }
        current_client = current_client->next;
    }
    return NULL;
}

// Функция для удаления клиента из хеш-таблицы
void remove_client(int id)
{
    int index = hash(id);
    t_client_info *current_client = hash_table[index];
    t_client_info *previous_client = NULL;

    while (current_client != NULL)
    {
        if (current_client->user->id == id)
        {
            // Если клиент первый в списке
            if (previous_client == NULL)
            {
                hash_table[index] = current_client->next;
            }
            // Если клиент не первый в списке
            else
            {
                previous_client->next = current_client->next;
            }
            free(current_client);
            return;
        }
        previous_client = current_client;
        current_client = current_client->next;
    }
}
