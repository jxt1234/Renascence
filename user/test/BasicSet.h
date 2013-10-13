struct BasicSet
{
    int size;
    void* content;
    BasicSet(){size=0; content = NULL;}
    BasicSet(int _size):size(_size){content=malloc(size);}
    ~BasicSet()
    {
        if (NULL!=content)
        {
            free(content);
        }
    }
};

