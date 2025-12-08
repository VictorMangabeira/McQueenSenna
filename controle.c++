/* Carrinho 2.0 - Atualizado para botões de segurar */

// Bibliotecas
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Nome e senha do servidor
const char* AC_SSID = "MCQueen Senna";
const char* AC_PWD = "12345678";

// Portas para controlar o motor A/ Primeiro motor
// Pinos do motor
int Pino1A = 26;
int Pino2A = 27;

// Portas para controlar o motor B; segundo motor
// Pinos do motor
int Pino1B = 32;
int Pino2B = 33;

//pinos Servo ******
int ataque = 13;
int defesa = 12;

//Variavel do servo
Servo servoAtaque;
Servo servoDefesa;

// Porta do servidor
WebServer server(80);

// Pagina que mostar ao acessar
const char* PAGINA = R"HTML(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controle do Carrinho Inteligente</title>
<style>
/* Fonte */
@import url('https://fonts.googleapis.com/css2?family=Silkscreen:wght@400;700&display=swap');

/* Configuração universal */
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    /* Remove o "toque azul" padrão ao clicar em botões no mobile */
    -webkit-tap-highlight-color: transparent;
    user-select: none; /* Importante para não selecionar texto ao segurar */
}

/* Corpo da página */
body {
    font-family: "Silkscreen", sans-serif;
    font-weight: 500;
    background: linear-gradient(90deg, #F9D616 0%, #FF8F00 100%);
    line-height: 1.95rem;
    min-height: 100vh;
}

/* Cabeçalho da página */
header {
    background: linear-gradient(90deg, #FF8F00 0%, #CE1620 100%);
    border: 1px ridge #FF0000;
    border-radius: 5px;
    padding: 5px;
    text-align: center;
    box-shadow: 0 5px 8px rgba(255, 145, 0, 0.555);
    margin: 10px auto;
    max-width: 90%;
}

header h1 {
    font-size: 1.5rem;
    margin: 5px;
    color: #48090E;
}

/* Configuração central/principal */
main {
    text-align: center;
    margin: 20px auto;
    padding: 10px;
    max-width: 950px;
    width: 100%;
}

/* ====== Seções dos botões ====== */
.Controle {
    display: grid;
    grid-template-columns: repeat(8, 1fr);
    grid-template-rows: repeat(3, 1fr);
    max-width: 950px;
    width: 100%;
    height: auto;
    margin: 0 auto;
    gap: 15px;
    justify-content: center;
    padding: 10px;
}

/* Efeito de clique e visual Ativo */
button:active, button.ativo {
    box-shadow: 0 2px rgba(0,0,0,0.2) !important;
    transform: translateY(4px);
    background-color: #ffaa33; /* Feedback visual */
}

/* Estilo visual dos botões */
section div button {
    font-size: 1.5rem;
    background-color: #FF8F00;
    border-radius: 15px;
    padding: 15px 0;
    border-color: #F9D616;
    color: rgb(72, 9, 14);
    box-shadow: 0 6px 0 #b86800;
    border: 2px solid #F9D616;
    font-weight: bold;
    width: 100%;
    height: 100%;
    cursor: pointer;
    touch-action: manipulation;
}

/* --- Posicionamento dos botões --- */
.botão-frente   { grid-column: 2/3; grid-row: 1/2; }
.botão-re       { grid-column: 2/3; grid-row: 3/4; }
.botão-direita  { grid-column: 3/4; grid-row: 2/3; }
.botão-esquerda { grid-column: 1/2; grid-row: 2/3; }
.botão-parar    { grid-column: 2/3; grid-row: 2/3; background-color: #CE1620 !important; color: white !important; }
.botão-ataque   { grid-column: 6/7; grid-row: 2/3; }
.botão-defesa   { grid-column: 7/8; grid-row: 2/3; }

/* Configuração do rodapé padrão */
footer {
    text-align: center;
    padding: 10px;
    background: linear-gradient(90deg,  #48090E 0%, #CE1620 100%);
    font-size: 0.8rem;
    flex-shrink: 0;
}

/* RESPONSIVIDADE - CELULAR */
@media (max-width: 600px) {
    body { overflow-x: hidden; }
    header h1 { font-size: 1.2rem; }
    footer { padding: 2px 0; font-size: 0.6rem; line-height: 1; border-top: 1px solid #48090E; }
    
    .Controle {
        grid-template-columns: repeat(3, 1fr);
        grid-template-rows: repeat(5, 80px);
        gap: 10px;
    }

    .botão-frente   { grid-column: 2/3; grid-row: 1/2; }
    .botão-esquerda { grid-column: 1/2; grid-row: 2/3; }
    .botão-parar    { grid-column: 2/3; grid-row: 2/3; }
    .botão-direita  { grid-column: 3/4; grid-row: 2/3; }
    .botão-re       { grid-column: 2/3; grid-row: 3/4; }
    
    .botão-ataque { grid-column: 1/2; grid-row: 5/6; margin-top: 20px; }
    .botão-defesa { grid-column: 3/4; grid-row: 5/6; margin-top: 20px; }
    
    section div button { font-size: 1.5rem; box-shadow: 0 4px 0 #b86800; }
}

@media (max-height: 500px) and (orientation: landscape) {
    header { display: none; }
    footer { display: none; }
    main { margin: 5px auto; }
    .Controle {
        grid-template-columns: repeat(8, 1fr);
        grid-template-rows: repeat(3, 1fr);
        height: 85vh;
        gap: 5px;
    }
}
</style>
</head>
<body>
    <header>
        <h1>Controle Remoto</h1> 
    </header>
    <main>
        <section>
            <div class="Controle">
                <button id="btnFrente" class="botão-frente">⬆</button>
                <button id="btnEsquerda" class="botão-esquerda">⬅</button>
                <button id="btnDireita" class="botão-direita">➡</button>
                <button id="btnRe" class="botão-re">⬇</button>
                
                <button id="btnParar" class="botão-parar" onclick="enviar('/parar')">⏹</button>
                
                <button id="btnAtaque" class="botão-ataque">🗡</button>
                <button id="btnDefesa" class="botão-defesa">🛡</button>
            </div>
        </section>
    </main>
    <footer>
        <p>Projeto integrador IoT01</p>
        <p>Página desenvolvida por Victor, Luis e José</p>
    </footer>

    <script>
        // Função para enviar comandos sem recarregar a tela
        function enviar(acao) {
            fetch(acao).catch(err => console.log(err));
        }

        // Função que configura o "Segurar para Ativar"
        function configurarBotao(id, acaoApertar, acaoSoltar) {
            const btn = document.getElementById(id);
            if (!btn) return;

            // Para Celular (Touch)
            btn.addEventListener('touchstart', (e) => { 
                e.preventDefault(); 
                btn.classList.add('ativo'); 
                enviar(acaoApertar); 
            });
            btn.addEventListener('touchend', (e) => { 
                e.preventDefault(); 
                btn.classList.remove('ativo'); 
                enviar(acaoSoltar); 
            });

            // Para Computador (Mouse)
            btn.addEventListener('mousedown', (e) => { e.preventDefault(); enviar(acaoApertar); });
            btn.addEventListener('mouseup',   (e) => { e.preventDefault(); enviar(acaoSoltar); });
            btn.addEventListener('mouseleave',(e) => { e.preventDefault(); enviar(acaoSoltar); });
        }

        // --- Configurando todos os botões ---
        
        // Movimento (Segura para andar, solta para parar)
        configurarBotao('btnFrente',   '/frente',   '/parar');
        configurarBotao('btnRe',       '/re',       '/parar');
        configurarBotao('btnEsquerda', '/esquerda', '/parar');
        configurarBotao('btnDireita',  '/direita',  '/parar');

        // Ações (Segura para ativar, solta para voltar)
        configurarBotao('btnAtaque', '/ataque', '/ataqueoff');
        configurarBotao('btnDefesa', '/defesa', '/defesaoff');

    </script>
</body>
</html>
)HTML";

//Executa o HTML'
void handleRoot() {
  server.send(200, "text/html", PAGINA);
}

// Resposta simples para não travar o wifi
void responderOK() {
  server.send(200, "text/plain", "OK");
}

// --- FUNÇÕES DE CONTROLE ---

void motorFrente() {
  digitalWrite(Pino1A, HIGH); digitalWrite(Pino2A, LOW);
  digitalWrite(Pino1B, HIGH); digitalWrite(Pino2B, LOW);
  responderOK();
}

void motorRe() {
  digitalWrite(Pino1A, LOW); digitalWrite(Pino2A, HIGH);
  digitalWrite(Pino1B, LOW); digitalWrite(Pino2B, HIGH);
  responderOK();
}

void motorEsquerda() {
  digitalWrite(Pino1A, LOW); digitalWrite(Pino2A, HIGH);
  digitalWrite(Pino1B, HIGH); digitalWrite(Pino2B, LOW); // Eixo invertido para girar no eixo
  responderOK();
}

void motorDireita() {
  digitalWrite(Pino1A, HIGH); digitalWrite(Pino2A, LOW);
  digitalWrite(Pino1B, LOW); digitalWrite(Pino2B, HIGH); // Eixo invertido para girar no eixo
  responderOK();
}

void motorParar() {
  digitalWrite(Pino1A, LOW); digitalWrite(Pino2A, LOW);
  digitalWrite(Pino1B, LOW); digitalWrite(Pino2B, LOW);
  responderOK();
}

// --- FUNÇÕES DE ATAQUE E DEFESA ---

// Ativa o ataque (Botão apertado)
void sAtaque() {
  servoAtaque.write(90); 
  responderOK();
}
// Desativa o ataque (Botão solto) - NOVA
void sAtaqueParar() {
  servoAtaque.write(0); 
  responderOK();
}

// Ativa a defesa (Botão apertado)
void sDefesa() {
  servoDefesa.write(180); 
  responderOK();
}
// Desativa a defesa (Botão solto) - NOVA
void sDefesaParar() {
  servoDefesa.write(90); 
  responderOK();
}


void setup() {
  // Configurando as portas
  pinMode(Pino1A, OUTPUT); pinMode(Pino2A, OUTPUT);
  pinMode(Pino1B, OUTPUT); pinMode(Pino2B, OUTPUT);

  // Pinos do Servo
  servoAtaque.attach(ataque, 500, 2400);
  servoDefesa.attach(defesa, 500, 2400);
  
  // WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AC_SSID, AC_PWD);

  Serial.begin(115200);
  Serial.println("Criando rede WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Rotas
  server.on("/", handleRoot);
  
  // Movimento
  server.on("/frente", motorFrente);
  server.on("/re", motorRe);
  server.on("/parar", motorParar);
  server.on("/direita", motorDireita);
  server.on("/esquerda", motorEsquerda);
  
  // Ação (Ligado)
  server.on("/ataque", sAtaque);
  server.on("/defesa", sDefesa);
  
  // Ação (Desligado - Novo)
  server.on("/ataqueoff", sAtaqueParar);
  server.on("/defesaoff", sDefesaParar);

  // Posição inicial
  servoAtaque.write(0);
  servoDefesa.write(90);
  
  server.begin();
}

void loop() {
  server.handleClient();
}