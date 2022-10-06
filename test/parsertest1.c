struct Complex {
    int real;
};

int main()
{
    struct Complex x;
    x.real = 1;

    while (x.real < 10) {
        x.real = x.real + 1;
    }
}
