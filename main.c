#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int code;
    char name[51];
    char cpf[12];
    char registryNumber[31];
    char address[101];
    char phone[20];
} Professional;

typedef struct {
    int top;
    int free;
} HeaderData;

void writeHeader(FILE* file, HeaderData* header) {
    fseek(file, 0, SEEK_SET);
    fwrite(header, sizeof(HeaderData), 1, file);
}

HeaderData* readHeader(FILE* file) {
    HeaderData* header = malloc(sizeof(HeaderData));
    fseek(file,0, SEEK_SET);
    int success = fread(header, sizeof(HeaderData), 1, file);
    if (!success) {
        header->top = 0;
        header->free = -1;
        writeHeader(file,header);
    }
    return header;
}

Professional* getProfessional(FILE* file, int pos) {
    Professional* p = malloc(sizeof(Professional));
    fseek(file,sizeof(HeaderData) + pos * sizeof(Professional), SEEK_SET);
    fread(p, sizeof(Professional), 1, file);
    return p;
}

void writeProfessional(FILE* file, Professional* professional, int pos) {
    fseek(file, sizeof(HeaderData) + pos * sizeof(Professional), SEEK_SET);
    fwrite(professional, sizeof(Professional), 1, file);
}

int insertProfessional(Professional* professional, FILE* file) {
    HeaderData* header = readHeader(file);
    int pos;
    if (header->free == -1) {
        pos = header->top;
        header->top++;
    } else {
        pos = header->free;
        Professional* aux = getProfessional(file,header->free);
        header->free = aux->code;
        free(aux);
    }
    writeProfessional(file,professional,pos);
    writeHeader(file, header);
    free(header);
    return pos;
}

void removeProfessional(int pos, FILE* file) {
    HeaderData* header = readHeader(file);
    Professional* p = getProfessional(file,pos);
    p->code = header->free;
    header->free = pos;
    writeHeader(file,header);
    writeProfessional(file, p, pos);
    free(header);
    free(p);
}

Professional* requestProfessional() {
    Professional* p = malloc(sizeof(Professional));
    printf("Insira os dados do profissional\n");
    printf("Codigo: ");
    scanf("%d%*c", &p->code);
    printf("Nome: ");
    fgets(p->name, 51, stdin);
    printf("CPF: ");
    fgets(p->cpf, 12, stdin);
    printf("Numero de Registro: ");
    fgets(p->registryNumber,31 , stdin);
    printf("Endereco: ");
    fgets(p->address, 101, stdin);
    printf("Telefone: ");
    fgets(p->phone, 30, stdin);
    return p;
}
void printDatafile(FILE* file) {
    HeaderData* h = readHeader(file);
    printf("Topo => %d| Livre => %d\n",h->top,h->free);
    Professional* p = malloc(sizeof(Professional));
    while (fread(p,sizeof(Professional), 1, file)) {
        printf("Codigo => %d\n", p->code);
    }
    free(h);
    free(p);
}

int main() {
    FILE* f = fopen("test.txt","r+b");
    Professional* p = requestProfessional();
    int result = insertProfessional(p,f);
    //HeaderData h = {4,-1};
    //writeHeader(f,&h);
    printDatafile(f);
    return 0;
}