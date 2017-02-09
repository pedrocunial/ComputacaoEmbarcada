#include <stdio.h>
#include <stdlib.h>

typedef struct type_product
{
    char name[20];
    unsigned short qtd;
    float price;
} product;


product* create_table()
{
    unsigned short n_products;
    product* products;

    printf("Quantas comidas você comprou?\n");
    scanf("%d", &n_products);
    products = malloc(n_products * sizeof(product));

    for (n_products; n_products--;) {
        printf("Entre o nome do produto:\n");
        scanf("%s", &products[n_products].name);
        printf("Entre o preco unitario do produto:\n");
        scanf("%f", &products[n_products].price);
        printf("Entre a quantidade comprada:\n");
        scanf("%d", &products[n_products].qtd);
    }
    return products;
}

int main()
{
    product* products = create_table();
    unsigned short n_products = sizeof(products) / sizeof(products[0]);

    printf("| Nome do produto | Preço Unitário | Quantidade comprada |");
    printf(" Total do Produto |\n");

    for (n_products; n_products--;) {
        printf("| %s | %f | %d | %f |",
               products[n_products].name,
               products[n_products].price,
               products[n_products].qtd,
               products[n_products].price * products[n_products].qtd);
    }
    return 0;
}
