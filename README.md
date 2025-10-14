<img width=100% src="https://capsule-render.vercel.app/api?type=waving&color=66CDAA&height=120&section=header"/>

# ZOMBIE RAMPAGE 🧟

Projeto desenvolvido para a disciplina de **Algoritmos**, utilizando a linguagem **C** e os princípios fundamentais da **Programação Orientada a Objetos e Algoritmos**.

---

## 📌 Objetivo

O projeto tem como objetivo o desenvolvimento de um jogo 2D RPG com elementos de estratégia, dividido em duas etapas de complexidade crescente, utilizando a linguagem C como base.
O jogo iniciou em uma versão textual (terminal), com foco nos conceitos fundamentais de programação em C, e promete evoluir para uma versão gráfica 2D, implementando estruturas de dados, algoritmos e elementos de IA.
O tema escolhido é pós-apocalíptico, em um mundo devastado por um vírus que transforma humanos em zumbis. O jogador assume o papel de um sobrevivente que precisa explorar, lutar e administrar recursos para permanecer vivo.

---

## 👨‍💻 Integrantes da Equipe

- Bruno Gabriel `<bgprs>`
- Diogo da Silva `<dsr>`
- Gryghor Camonni `<gcfc>`
- Flávia Vitória `<fves>`
- Lucas Cabral `<lsc>`

---

## 🧱 Tecnologias e Ferramentas

- 🖥️ Linguagem: **C**
- 💾 Paradigma: **Programação Orientada a Objetos e Algoritmos**
- 📋 Documentação: **Google Docs, GitHub**
- 🎨 Apresentação visual: **Canva, Miro**
- 🌐 Interface Web: **Em andamento**

---

## 🧩 Arquitetura do Sistema

O sistema é composto pelas seguintes classes:

- `Batalha` Estrutura de Fila onde o Jogador e inimigo alternam ataques e defesas. O sistema de dano é baseado em atributos da classe, itens e níveis.
- `Inventario`: Vetor Dinâmico. Armazenamento e manipulação de itens coletados.
- `Mapa`: Matriz (2D array). Representação dos locais e movimentação. 
- `Inimigo`: Algoritmos de busca (BFS, A*). Inimigos perseguem o jogador no mapa.
- `Personagem`: Mantém as informações do jogador, seu XP e itens atráves do salvamento com arquivos binários.

### 🧠 Princípios de POO Aplicados

| Conceito         | Como foi aplicado                                              | Benefício                            |
|------------------|----------------------------------------------------------------|---------------------------------------|
| **Struct**       | Para personagem, itens e inimigos                   | Reutilização de código e hierarquia   |
| **Funções**  | para modularização do código                          | Flexibilidade para múltiplos ajustes futuros   |
| **ponteiros e malloc()**| Na lista de armazenamento de itens do inventário              | para manipular memória dinamicamente|
| **Arquivos (fopen, fwrite, fread)**     | para salvar progresso| Facilidade de uso e manutenção        |
| **Variáveis globais**     | para estados de jogo (nível, HP, XP)| Facilidade de uso e atualizações futuras        |

---

## 🔄 Fluxo de Uso (Resumo)

1. Interface por texto (menus e opções via terminal).
2. Criação e seleção de personagem.
3. Sistema simples de batalhas em turnos (jogador vs zumbi).
4. Inventário básico (adição, remoção e listagem de itens).
5. Mapa representado em matriz textual (# = obstáculo, Z = zumbi, P = player).
6. Salvamento e carregamento de progresso via arquivos texto/binários.

---

## 🌍 Interface Web (opcional)

Você pode acessar a versão do site em:  
🔗 Em andamento...

> *Em andamento...*

---

## Passos para rodar o projeto localmente

1. Estar Utilizando o Sistema Operacional Windows

2. Clone o repositório:
```bash
git clone https://github.com/LucasCabra7/Algorithm-group-2.git
```
3. Navegue até o diretório do projeto
```bash
cd Algorithm-group-2
```
4. Compile o projeto
```bash
gcc src/*.c -I./include -o zombie
```
5. 4. Execute o programa
```bash
./zombie.exe
```




## Galeria de Projetos



## 📎 Links Úteis

- 📒 Documentos Google (Documentação): [[Acessar](https://miro.com/app/board/uXjVIjGJgRI=/?share_link_id=360641450079)](https://docs.google.com/document/d/1AzQkcqnJ7S7V7j1aGL4Yy1mdcivLg5VonMRJQ03T9Tw/edit?usp=sharing)

---

## 📃 Licença

Este projeto é de caráter acadêmico, sem fins lucrativos. Todos os direitos reservados aos autores.

## Equipe do Projeto

<div align="center">

  <table>
    <tr>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/162474087?v=4" width="100px" alt="Pessoa 1"/><br/>
        <b>Bruno Ramos 1</b>
      </td>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/155683708?v=4" width="100px" alt="Lucas Cabral"/><br/>
        <b>Lucas Cabral</b>
      </td>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/73610632?v=4" width="100px" alt="Gryghor"/><br/>
        <b>Gryghor Camonni</b>
      </td>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/149613054?v=4" width="100px" alt="Pessoa 3"/><br/>
        <b>Diogo Rodrigues</b>
      </td>
      <td align="center">
        <img src="https://avatars.githubusercontent.com/u/205646287?v=4" width="100px" alt="Flavitche"/><br/>
        <b>Flávia Vitória</b>
      </td>
    </tr>
  </table>

</div>

---

<p align="center">
  &copy; 2025 Universidade Federal de Pernambuco - Centro de Informática. Todos os direitos reservados.
</p>

<img width=100% src="https://capsule-render.vercel.app/api?type=waving&color=66CDAA&height=120&section=header"/>
