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
- 📞 Comunicação: **Discord, Whatsapp e Github projects**
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
2. Criação e seleção de personagem (Soldado, Engenheiro ou Médico).
3. **NOVO:** Sistema de overworld expandido (25x20) com exploração livre.
4. **NOVO:** Múltiplos tipos de terreno (grama, árvores, água, prédios, obstáculos).
5. Sistema de batalhas em turnos (jogador vs zumbi) ao encontrar inimigos.
6. Inventário dinâmico com múltiplos tipos de itens.
7. **NOVO:** Três tipos de itens colecionáveis:
   - `+` Medkit (restaura HP)
   - `!` Armas (aumenta ataque permanentemente)
   - `^` Munição (recursos de combate)
8. Sistema de progressão com XP e níveis.
9. Salvamento e carregamento de progresso via arquivos binários.
10. **NOVO:** Tutorial in-game acessível a qualquer momento (tecla 'h').

### 🎮 Controles do Jogo

- `w/a/s/d` - Mover para cima/esquerda/baixo/direita
- `i` - Ver inventário
- `p` - Ver status do personagem
- `h` - Mostrar ajuda/tutorial
- `q` - Salvar jogo
- `e` - Voltar ao menu principal

### 🗺️ Elementos do Mapa

| Símbolo | Descrição | Pode Atravessar? |
|---------|-----------|------------------|
| `P` | Jogador (você) | - |
| `Z` | Zumbi (inicia combate) | Sim |
| `+` | Medkit (cura HP) | Sim |
| `!` | Arma (aumenta ataque) | Sim |
| `^` | Munição | Sim |
| `#` | Obstáculo/Rocha | Não |
| `T` | Árvore | Não |
| `~` | Água | Não |
| `B` | Prédio | Não |
| `,` | Grama | Sim |
| `.` | Chão livre | Sim |

> *Este é o coração de nosso projeto algumas features estão sujeitas à alterações, porém estamos em trabalho contínuo para melhorar a sua experiencia , sinta-se livre para dar sugestões...*
---

## ✨ Novidades da Versão Atual

### Sistema de Overworld Expandido
- **Mapa Maior:** Expandido de 10x10 para 25x20 tiles, oferecendo mais espaço para exploração
- **Terrenos Variados:** Adicionados múltiplos tipos de terreno (grama, árvores, água, prédios abandonados)
- **Geração Procedural:** Cada novo jogo cria um mapa único com distribuição aleatória de terrenos e itens

### Melhorias de Gameplay
- **Três Tipos de Itens:** Medkits, Armas e Munição distribuídos pelo mapa
- **Feedback Aprimorado:** Mensagens descritivas quando não é possível mover (ex: "Você não pode atravessar água!")
- **Estatísticas em Tempo Real:** Visualização de zumbis restantes e itens disponíveis no mapa
- **Tutorial In-Game:** Ajuda acessível a qualquer momento pressionando 'h'
- **Progressão Visual:** Barra de XP exibida no HUD do jogo

### Melhorias de Interface
- **HUD Informativo:** Exibe posição, HP, nível, XP e contadores de objetivos
- **Legenda Clara:** Explicação de todos os símbolos do mapa
- **Mensagens de Boas-Vindas:** Introdução narrativa ao iniciar novo jogo

---

## 🌍 Interface Web (opcional)

Você pode acessar a versão do site em:  
🔗 Em andamento...

> *Em andamento...*

---

## Passos para rodar o projeto localmente

### Windows

1. Clone o repositório:
```bash
git clone https://github.com/Monkius-Maximus/Algorithm-group-2-diogoversion.git
```

2. Navegue até o diretório do projeto:
```bash
cd Algorithm-group-2-diogoversion
```

3. Compile o projeto:
```bash
gcc src/*.c -I./include -o zombie.exe
```

4. Execute o programa:
```bash
zombie.exe
```

### Linux/macOS

1. Clone o repositório:
```bash
git clone https://github.com/Monkius-Maximus/Algorithm-group-2-diogoversion.git
```

2. Navegue até o diretório do projeto:
```bash
cd Algorithm-group-2-diogoversion
```

3. Compile o projeto:
```bash
gcc src/*.c -I./include -o zombie
```

4. Execute o programa:
```bash
./zombie
```




## Galeria de Projetos



## 📎 Links Úteis

- 📒 Documentos Google (Documentação): [[Acessar](https://docs.google.com/document/d/1AzQkcqnJ7S7V7j1aGL4Yy1mdcivLg5VonMRJQ03T9Tw/edit?usp=sharing)]

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
