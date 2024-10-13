#include <stdio.h>
#include <stdlib.h>

#define MAX_COLUNAS 103
#define MAX_LINHAS 43

 //Structs gerais
typedef struct {
	int x;
	int y;
} tJogador;

typedef struct {
	int linha;
	int ordem;
} tRastreio;

typedef struct {
	int x;
	int y;
	int fileira;
	int id;
	int indice;
	int vida;
	int ordemM;
	int iteracao;
	int yMorte;
	int xMorte;
} tInimigo;

typedef struct {
	int x;
	int y;
	int ativo;
} tTiro;

typedef struct {
	int qtdMovimento;
	int interacoes;
	int pontos;
	int tirosEfetivos;
	int tirosErrados;
	int descidaInimiga;
} tEstatisticas;

typedef struct {
	tJogador jogador;
	tInimigo inimigo[100];
	tEstatisticas stats;
	tTiro tiro;
	int vitoria;
	int derrota;
	int numInimigos;
	int direcao;
	int linhas;
	int colunas;
	char matriz[MAX_LINHAS][MAX_COLUNAS];
	int heatMap[MAX_LINHAS][MAX_COLUNAS];
	char formato[3][4];
	char formato2[3][4];
	char formato3[3][4];
	int qualFormato;
	int bonus;
	int ordem;
} tMapa;




//Inicializaz struct mapa
tMapa InicializaMapa(tMapa mapa);

//Funcoes relacionadas ao Mapa
tMapa MontaBordasMapa(tMapa mapa);

tMapa LeArqMapa(tMapa mapa, char diretorio[]);


//Funcoes relacionadas ao Jogador
int TransfereDadosXJogador(tJogador player);

int TransfereDadosYJogador(tJogador player);

tMapa CriaPlayer(tMapa mapa, FILE * ArqMapa);

tMapa DesenhandoJogador(tMapa mapa);


//Funcoes relacionadas ao inimigo

tMapa LeArqInimigo(tMapa mapa, char diretorio[]);

tMapa TransfereDadosInimigo(tMapa mapa, int id, int x, int y, int fileira, int indice);

tMapa CriaEnemy(tMapa mapa, FILE * ArqMapa);

tMapa DesenhaInimigoEstatico(tMapa mapa);


//Funcao do mapa de calor
tMapa AdicionaJogadorNoCalor(tMapa mapa);

//Printa o mapa
void EscreveMapa(tMapa mapa);

void MostraMapaJogo(tMapa mapa);

//Funcao pra ordenar de forma crescente as linhas
int EstaCrescente(tRastreio rastreio[], int tam);

void OrdenaCrescenteLinhas(tRastreio rastreio[], int tam);


//Funcao do arquivo de inicializacao
void GeraArqInicio(tMapa mapa, char diretorio[]);

//Funcao pra abrir o arquivo de ranking
void AbreArqRanking(tMapa mapa, char diretorio[]);

//Funcao pra criar o arquivo de estatisticas
void GeraArqEstatisticas (tMapa mapa, char diretorio[]);

//Funcao pra criar o arquivo de resumo
FILE * CriaArqResumo(char diretorio[]);

//Funcao pra criar o arquivo do mapa de calor
void GeraArqHeatMap(tMapa mapa, char diretorio[]);


//Funcoes relacionadas a inicializacao
tMapa InicializaJogo(tMapa mapa, char diretorio[]);


//Funcoes do tiro
tMapa DesativaTiro(tMapa mapa);

tMapa TransfereDadosTiro(tMapa mapa, tTiro tiro);

int TiroAtivo(tMapa mapa);

tMapa MovimentaTiro(tMapa mapa);

tMapa EfetuaTiro(tMapa mapa);

int Acertou(tMapa mapa, int yTiro, int xTiro, int yInimigo, int xInimigo);

tMapa TiroAcertou(tMapa mapa, FILE * ArquivoResumo);


//Funcoes de movimentacao em geral
tMapa InimigoVivo(tMapa mapa);

int TrocaDeDirecao(tMapa mapa, FILE * ArquivoResumo);

tMapa MovimentaInimigos(tMapa mapa, FILE * ArquivoResumo);

tMapa EfetuaMovimentoJogador(tMapa mapa, char jogada, FILE * ArquivoResumo);

tMapa JogadorMoveDireita(tMapa mapa, FILE * ArquivoResumo);

tMapa JogadorMoveEsquerda(tMapa mapa, FILE * ArquivoResumo);


//Funcoes de execucao do jogo 
int JogadorVenceu(tMapa mapa);

int InimigosNoFim(tMapa mapa);

tMapa LeArqJogadas(tMapa mapa, char diretorio[]);

tMapa ComecaJogo(tMapa mapa, char diretorio[]);

//Inicializa HeatMap
tMapa ZeraMapaDeCalor(tMapa mapa);


int main(int argc, char * argv[]){
	tMapa mapa;
	
	mapa = InicializaMapa(mapa);
	
	
	if(argc <= 1){
		printf("ERRO: Informe o diretorio com os arquivos de configuracao.");
		exit(0);
	}
	
		
	//analisa o arquivo de mapa e de inimigos, depois imprime tudo
	mapa = InicializaJogo(mapa, argv[1]);
	
	mapa = ZeraMapaDeCalor(mapa);
	
	//zera o placar
	mapa.stats.interacoes = 0;
	mapa.stats.pontos = 0;
	
	//Executa o jogo
	mapa = ComecaJogo(mapa, argv[1]);
	
	
	return 0;
}

tMapa InicializaMapa(tMapa mapa){
	
	mapa.vitoria = 0;
    mapa.derrota = 0;
    mapa.numInimigos = 0;
    mapa.direcao = 1;
    mapa.linhas = 0;
    mapa.colunas = 0;
	mapa.tiro.ativo=0;
	mapa.tiro.x = 0;
	mapa.tiro.y = 0;
	mapa.ordem = 1;
	mapa.qualFormato = 1;
	mapa.bonus = 0;
	mapa.stats.tirosEfetivos = 0;
	mapa.stats.tirosErrados = 0;
	mapa.stats.qtdMovimento = 0;
	mapa.stats.descidaInimiga = 0;
	
	return mapa;
}

tMapa ZeraMapaDeCalor(tMapa mapa){
	int i, j, linhas=0, colunas=0;
	
	linhas = mapa.linhas;
	colunas = mapa.colunas;
	
	for(i=1; i < linhas; i++){
		for(j=1; j < colunas; j++){
			mapa.heatMap[i][j] = 0;
		}
	}
	
	
	return mapa;
}



tMapa MontaBordasMapa(tMapa mapa){
	int i, j;
	int linhas = mapa.linhas;
	int colunas = mapa.colunas;
	
	//preenche mapa de calor tmb
	
	//Preenche o Mapa
		for(i=0; i <= linhas; i++){
			for(j=0; j <= colunas; j++){
				mapa.matriz[i][j] = ' ';
			}
		}
			
	//Topo e Base do mapa
		for(i=0; i <= linhas; i++){
				
			if(i == 1 || i == linhas){
					
				for(j = 1; j <= colunas; j++){
						
					if((j == 1 || j == colunas) && (i == 1 || i == linhas)){
						mapa.matriz[i][j] = '+';
					} else {
						mapa.matriz[i][j] = '-';
					}
						
				}
			}
		}
					
					
	 //Laterais do mapa
		for(j=0; j <= colunas; j++){
				
			if(j == 1 || j == colunas){
					
				for(i=1; i <= linhas; i++){
						
					if(i == mapa.jogador.y - 1){
						mapa.matriz[i][j] = '-';
					} else {
						if(mapa.matriz[i][j] != '+'){
							mapa.matriz[i][j] = '|';
						}
					}
						
				}
			}
		}
	
	return mapa;
	
}

int TransfereDadosYJogador(tJogador player){
	
	return player.y;
}

int TransfereDadosXJogador(tJogador player){
	
	return player.x;
}


tMapa CriaPlayer(tMapa mapa, FILE * ArqMapa){
	tJogador player;
	char c;
	
	while(!feof(ArqMapa)){
		fscanf(ArqMapa,"%c", &c);
		if(c == '('){
			fscanf(ArqMapa, "%d %d", &player.x, &player.y);
		
		} else if (c == ')'){
			fscanf(ArqMapa,"%*[^\n]");
			fscanf(ArqMapa,"%*c");
			break;
		}
	}
	
	//Passar os dados pra estrutura principal
	mapa.jogador.x = TransfereDadosXJogador(player);
	mapa.jogador.y = TransfereDadosYJogador(player);
	
	return mapa;
}

tMapa DesenhaInimigoEstatico(tMapa mapa){
	int i;
	int Xcentral = 0;
	int Ycentral = 0;
	
	if(mapa.numInimigos == 0){
		
	} else {
		for(i=0; i < mapa.numInimigos; i++){
		
		Xcentral = mapa.inimigo[i].x + 1;
		Ycentral = mapa.inimigo[i].y + 1;
		
			if(mapa.inimigo[i].vida == 1){
				
				if(mapa.qualFormato == 1){
					//centro do inimgo
					mapa.matriz[Ycentral][Xcentral] = mapa.formato[1][1];
			
					mapa.matriz[Ycentral-1][Xcentral] = mapa.formato[0][1];
					mapa.matriz[Ycentral-1][Xcentral-1] = mapa.formato[0][0];
					mapa.matriz[Ycentral-1][Xcentral+1] = mapa.formato[0][2];
					mapa.matriz[Ycentral+1][Xcentral] = mapa.formato[2][1];
					mapa.matriz[Ycentral+1][Xcentral-1] = mapa.formato[2][0];
					mapa.matriz[Ycentral+1][Xcentral+1] = mapa.formato[2][2];
					mapa.matriz[Ycentral][Xcentral-1] = mapa.formato[1][0];
					mapa.matriz[Ycentral][Xcentral+1] = mapa.formato[1][2];
					
				} else if(mapa.qualFormato == 2){
					//centro do inimgo
					mapa.matriz[Ycentral][Xcentral] = mapa.formato2[1][1];
			
					mapa.matriz[Ycentral-1][Xcentral] = mapa.formato2[0][1];
					mapa.matriz[Ycentral-1][Xcentral-1] = mapa.formato2[0][0];
					mapa.matriz[Ycentral-1][Xcentral+1] = mapa.formato2[0][2];
					mapa.matriz[Ycentral+1][Xcentral] = mapa.formato2[2][1];
					mapa.matriz[Ycentral+1][Xcentral-1] = mapa.formato2[2][0];
					mapa.matriz[Ycentral+1][Xcentral+1] = mapa.formato2[2][2];
					mapa.matriz[Ycentral][Xcentral-1] = mapa.formato2[1][0];
					mapa.matriz[Ycentral][Xcentral+1] = mapa.formato2[1][2];
					
				} else if (mapa.qualFormato == 3){
					
					//centro do inimgo
					mapa.matriz[Ycentral][Xcentral] = mapa.formato3[1][1];
			
					mapa.matriz[Ycentral-1][Xcentral] = mapa.formato3[0][1];
					mapa.matriz[Ycentral-1][Xcentral-1] = mapa.formato3[0][0];
					mapa.matriz[Ycentral-1][Xcentral+1] = mapa.formato3[0][2];
					mapa.matriz[Ycentral+1][Xcentral] = mapa.formato3[2][1];
					mapa.matriz[Ycentral+1][Xcentral-1] = mapa.formato3[2][0];
					mapa.matriz[Ycentral+1][Xcentral+1] = mapa.formato3[2][2];
					mapa.matriz[Ycentral][Xcentral-1] = mapa.formato3[1][0];
					mapa.matriz[Ycentral][Xcentral+1] = mapa.formato3[1][2];
				}
			}
		}
	}

	
	return mapa;
	
}

tMapa DesenhandoJogador(tMapa mapa){
	int Xprincipal = mapa.jogador.x;
	int Yprincipal = mapa.jogador.y;
	
	//centro jogador
	mapa.matriz[Yprincipal][Xprincipal] = 'M';
	
	mapa.matriz[Yprincipal + 1][Xprincipal] = 'M';
	mapa.matriz[Yprincipal + 2][Xprincipal] = 'M';
	mapa.matriz[Yprincipal + 1][Xprincipal + 1] = 'M';
	mapa.matriz[Yprincipal + 1][Xprincipal + 2] = 'M';
	mapa.matriz[Yprincipal + 2][Xprincipal + 1] = 'M';
	mapa.matriz[Yprincipal + 2][Xprincipal + 2] = 'M';
	mapa.matriz[Yprincipal][Xprincipal + 2] = 'M';
	
	return mapa;
}

tMapa TransfereDadosInimigo(tMapa mapa, int id, int x, int y, int fileira, int indice){
	mapa.inimigo[id].id = id;
	mapa.inimigo[id].x = x;
	mapa.inimigo[id].y = y;
	mapa.inimigo[id].fileira = fileira;
	mapa.inimigo[id].indice = indice;
	
	
	return mapa;
}

tMapa CriaEnemy(tMapa mapa, FILE * ArqMapa){
	tInimigo enemy;
	char c;
	int id=0;
	int fileira = 1;
	int qtdEnemy=0;
	int indice = 0;
	
	while(!feof(ArqMapa)){
		fscanf(ArqMapa, "%c", &c);
		if(c == '('){
			fscanf(ArqMapa, "%d %d", &enemy.x, &enemy.y);
			qtdEnemy++;
			indice++;
			mapa = TransfereDadosInimigo(mapa, id, enemy.x, enemy.y, fileira, indice);
			mapa.inimigo[id].vida = 1;
			id++;
		} else if (c == '\n'){
			fileira++;
			indice = 0;
		} else {
			continue;
		}
	}
	
	mapa.numInimigos = qtdEnemy;
	
	return mapa;
	
}

tMapa LeArqMapa(tMapa mapa, char diretorio[]){

	FILE * ArqMapa;
	int linhas=0, colunas=0;
	int rtn=0;
	char caminhoMapa[1000];
	
	//confeccao do acesso ao arquivo de mapa
	sprintf(caminhoMapa, "%s/mapa.txt", diretorio);
	
	//Ler Arquivo do Mapa
	ArqMapa = fopen(caminhoMapa, "r");
	
	if(!ArqMapa){
		printf("ERRO: nao foi possivel abrir arquivo %s\n", caminhoMapa);
		exit(0);
	} 
		
		// Montar a base do mapa
		rtn = fscanf(ArqMapa,"%d %d*[^\n]\n", &colunas, &linhas);
		
		if(rtn < 2){
			printf("ERRO: dados do arquivo nao foram coletados\n");
			exit(0);
		}
		 
			
			//ajuste de posicionamento
			mapa.linhas = linhas + 2;
			mapa.colunas = colunas + 2;
			
			
			//coleta de dados do jogador
			mapa = CriaPlayer(mapa, ArqMapa);
			
			//literalmente montando as bordas
			mapa = MontaBordasMapa(mapa);

			
			//Adciona Jogador
			mapa = DesenhandoJogador(mapa);
			
			//coleta de dado do inimigo
			mapa = CriaEnemy(mapa, ArqMapa);

			fclose(ArqMapa);
		
		
		
		
		
	
	
	return mapa;
	
}

tMapa LeArqInimigo(tMapa mapa, char diretorio[]){
	FILE * ArqInimigo;
	char caminhoInimigo[1000];
	char arquivoI[15] = {"inimigo.txt"};
	int i;
	
	
	//confeccao do acesso
	sprintf(caminhoInimigo, "%s/%s", diretorio, arquivoI);
	
	//acesso ao arquivo inimigo.txt
	ArqInimigo = fopen(caminhoInimigo, "r");
	
	if(!ArqInimigo){
		
		printf("ERRO: nao foi possivel abrir arquivo do inimigo\n");
		exit(0);
		
	} else {
		
		fscanf(ArqInimigo,"%d", &mapa.bonus);
		
		fscanf(ArqInimigo, "%*[^\n]");
		fscanf(ArqInimigo, "%*c");
		
		if(mapa.bonus == 0){
			for(i=0; i < 3; i++){
					fscanf(ArqInimigo, "%[^\n]", mapa.formato[i]);
					fscanf(ArqInimigo, "%*c");
			}
		} else if (mapa.bonus == 1){
			for(i=0; i < 3; i++){
				fscanf(ArqInimigo, "%[^\n]", mapa.formato[i]);
				fscanf(ArqInimigo, "%*c");
			}
			for(i=0; i < 3; i++){
				fscanf(ArqInimigo, "%[^\n]", mapa.formato2[i]);
				fscanf(ArqInimigo, "%*c");		
			}
			for(i=0; i < 3; i++){
					fscanf(ArqInimigo, "%[^\n]", mapa.formato3[i]);
					fscanf(ArqInimigo, "%*c");
			}
		}
	}
	
	fclose(ArqInimigo);
	
	
	mapa = DesenhaInimigoEstatico(mapa);
	

	return mapa;
}

void EscreveMapa(tMapa mapa){
	int i, j;
		
	for(i=1; i <= mapa.linhas; i++){
		for(j=1; j <= mapa.colunas; j++){
			printf("%c", mapa.matriz[i][j]);
		}
		printf("\n");
	}
}


void GeraArqInicio(tMapa mapa, char diretorio[]){
	int i, j;
	FILE * ArquivoI;
	
	char caminhoInicializacao[1000];
	
	sprintf(caminhoInicializacao, "%s/saida/inicializacao.txt", diretorio);
	
	ArquivoI = fopen(caminhoInicializacao,"w");
	
	if(!ArquivoI){
		printf("ERRO: problema criando o arquivo de inicializacao\n");
		exit(0);
	}
	
	for(i=1; i <= mapa.linhas; i++){
		for(j=1; j <= mapa.colunas; j++){
			fprintf(ArquivoI, "%c", mapa.matriz[i][j]);
		}
		fprintf(ArquivoI,"\n");
	}
	
	fprintf(ArquivoI,"A posicao central do jogador iniciara em (%d %d).", mapa.jogador.x, mapa.jogador.y);
	
	fclose(ArquivoI);
	
}

tMapa InicializaJogo(tMapa mapa, char diretorio[]){

	
	//Informacoes sobre o mapa.txt
	mapa = LeArqMapa(mapa, diretorio);
	
	//Informacoes sobre o inimgo.txt
	mapa = LeArqInimigo(mapa, diretorio);
	
	//Gera o arquivo de Inicializacao
	GeraArqInicio(mapa, diretorio);
		

	
	return mapa;
					
}

void MostraMapaJogo(tMapa mapa){
	int i, j;
		

	if(TiroAtivo(mapa)){
		
		mapa = MontaBordasMapa(mapa);
	
		mapa = DesenhandoJogador(mapa);
	
		mapa = DesenhaInimigoEstatico(mapa);
		
		mapa.matriz[mapa.tiro.y][mapa.tiro.x] = 'o';
	} else {
		mapa = MontaBordasMapa(mapa);
		
		mapa = DesenhandoJogador(mapa);
		
		mapa = DesenhaInimigoEstatico(mapa);
		
	}
	
	for(i=1; i <= mapa.linhas; i++){
		for(j=1; j <= mapa.colunas; j++){
			printf("%c", mapa.matriz[i][j]);
		}
		printf("\n");
	}
}



tMapa JogadorMoveDireita(tMapa mapa, FILE * ArquivoResumo){
	
	//verifica se vai ter colisao com a borda
	if(mapa.jogador.x == mapa.colunas - 3){
		//colisao
		//jogador nao se move
		fprintf(ArquivoResumo,"[%d] Jogador colidiu na lateral direita.\n", mapa.stats.interacoes);
	} else {
		mapa.jogador.x += 1;
		mapa.stats.qtdMovimento++;
	}
	
	return mapa;
}

tMapa JogadorMoveEsquerda(tMapa mapa, FILE * ArquivoResumo){
	
	//verifica se vai ter colisao com a borda
	if(mapa.jogador.x == 2){
		//colisao
		//jogador nao se move
		fprintf(ArquivoResumo,"[%d] Jogador colidiu na lateral esquerda.\n", mapa.stats.interacoes);
	} else {
		mapa.jogador.x -= 1;
		mapa.stats.qtdMovimento++;
	}
	
	return mapa;
}

int TiroAtivo(tMapa mapa){
	int ativo=0;
	
	ativo = mapa.tiro.ativo;
	
	//verifica se ja existe um tiro em execucao
	if(ativo == 1){
		return 1;
	} else if (ativo == 0){
		return 0;
	} else {
		printf("ERRO: Variavel de tiro ativo com problema\n");
		exit(0);
	}
}

tMapa TransfereDadosTiro(tMapa mapa, tTiro tiro){
	
	mapa.tiro.x = tiro.x;
	mapa.tiro.y = tiro.y;
	mapa.tiro.ativo = tiro.ativo;
	
	return mapa;
}

tMapa DesativaTiro(tMapa mapa){
	
	
	mapa.tiro.x = 0;
	mapa.tiro.y = 0;
	mapa.tiro.ativo = 0;

	
	return mapa;
}

tMapa MovimentaTiro(tMapa mapa){
	int y=0;
	
	if(TiroAtivo(mapa)){
	
		mapa.tiro.y -= 1;
		mapa.heatMap[mapa.tiro.y][mapa.tiro.x] +=1;
		
		y = mapa.tiro.y;
	
		if(y == 1){
			mapa = DesativaTiro(mapa);
			mapa.stats.tirosErrados++;
			mapa.heatMap[mapa.tiro.y][mapa.tiro.x] +=1;
		}
	}
	return mapa;
}

tMapa EfetuaTiro(tMapa mapa){
	tJogador player;
	tTiro tiro;
	
	player.x = mapa.jogador.x;
	player.y = mapa.jogador.y;
	
	if(!TiroAtivo(mapa)){
	
		tiro.x = player.x + 1;
		tiro.y = player.y - 1;
		tiro.ativo = 1;
		mapa.stats.tirosEfetivos++;
		
		//passa tudo pra estrutura principal
		mapa = TransfereDadosTiro(mapa, tiro);
		mapa.heatMap[mapa.tiro.y][mapa.tiro.x] +=1;
	} else {
		//jogador nao se mexe
	}
	
	
	return mapa;
}

tMapa EfetuaMovimentoJogador(tMapa mapa, char jogada, FILE * ArquivoResumo){
	
	
	if(jogada == 'd'){
		
		mapa = JogadorMoveDireita(mapa, ArquivoResumo);
		
	} else if(jogada == 'a'){
		
		mapa = JogadorMoveEsquerda(mapa, ArquivoResumo);
		
	} else if(jogada == 's') {
		
		//jogador passou a vez
		
	} else if(jogada == ' '){
		
		mapa = EfetuaTiro(mapa);
	}
	
	
	return mapa;
}

int TrocaDeDirecao(tMapa mapa, FILE * ArquivoResumo){
	int i, contador=0, limiteEsquerda=0;
	
	limiteEsquerda = 2;
	
	
	for(i=0; i < mapa.numInimigos; i++){
		//somente inimigos vivos
		if(mapa.inimigo[i].vida == 1){
			//indentificar pra qual lado os inimigos estao indo e determinar onde pode ser a colisao
			if(mapa.direcao > 0){
				if(mapa.inimigo[i].x == mapa.colunas - 3){
					//colisao
					fprintf(ArquivoResumo, "[%d] Inimigo de indice %d da fileira %d colidiu na lateral direita.\n", mapa.stats.interacoes - 1, mapa.inimigo[i].indice, mapa.inimigo[i].fileira);
					contador++;
				}
			} else if (mapa.direcao < 0){
				if(mapa.inimigo[i].x == limiteEsquerda){
					//colisao
					fprintf(ArquivoResumo, "[%d] Inimigo de indice %d da fileira %d colidiu na lateral esquerda.\n", mapa.stats.interacoes - 1, mapa.inimigo[i].indice, mapa.inimigo[i].fileira);
					contador++;
				}
			}
		}
	}
	
	if(contador == 0){
		return 0;
	} else {
		return 1;
	}
	
	
}

tMapa MovimentaInimigos(tMapa mapa, FILE * ArquivoResumo){
	int i;
	
	if(TrocaDeDirecao(mapa, ArquivoResumo)){
		mapa.stats.descidaInimiga++;
		mapa.direcao = mapa.direcao * (-1);
		for(i=0; i < mapa.numInimigos; i++){
			if(mapa.inimigo[i].vida == 1){
				mapa.inimigo[i].y += 1;
			}
		}	
	} else {
	
		for(i=0; i < mapa.numInimigos; i++){
			if(mapa.inimigo[i].vida == 1){
				mapa.inimigo[i].x += mapa.direcao;
			}
		}
	}
	
	return mapa;
}

int InimigosNoFim(tMapa mapa){
	int i, y=0, limite=0, contador = 0;
	
	limite = mapa.jogador.y - 1;
	
	for(i=0; i < mapa.numInimigos; i++){
		if(mapa.inimigo[i].vida == 1){
			y = mapa.inimigo[i].y + 1;
		
			if(y == limite){
				contador++;
			}
		}
	}
	
	if(contador == 0){
		return 0;
	} else {
		return 1;
	}
}

int Acertou(tMapa mapa, int yTiro, int xTiro, int yInimigo, int xInimigo){
			
			if(TiroAtivo(mapa)){
				//verifica se o tiro pegou em alguma parte do inimigo
				if(yInimigo-1 == yTiro && xInimigo == xTiro){
				
					return 1;
				
				} else if(yInimigo-1 == yTiro && xInimigo-1 == xTiro){
				
					return 1;
				
				} else if(yInimigo-1 == yTiro && xInimigo+1 == xTiro){
				
					return 1;
				
				} else if(yInimigo+1 == yTiro && xInimigo == xTiro){
				
					return 1;
				
				} else if(yInimigo+1 == yTiro && xInimigo-1 == xTiro){
				
					return 1;
				
				} else if(yInimigo+1 == yTiro && xInimigo+1 == xTiro){
				
					return 1;
				
				} else if(yInimigo == yTiro && xInimigo-1 == xTiro){
				
					return 1;
				
				} else if(yInimigo == yTiro && xInimigo+1 == xTiro){
				
					return 1;
				} else if(yInimigo == yTiro && xInimigo == xTiro){
				
					return 1;
				
				} else {
					return 0;
				}
			} else {
				return 0;
			}

	
}
	
	
	

tMapa TiroAcertou(tMapa mapa, FILE * ArquivoResumo){
	int i, yTiro=0, xTiro=0, yInimigo=0, xInimigo=0;
	
	yTiro = mapa.tiro.y;
	xTiro = mapa.tiro.x;
	
	for(i=0; i < mapa.numInimigos; i++){
		
		if(mapa.inimigo[i].vida == 1){
			yInimigo = mapa.inimigo[i].y + 1;
			xInimigo = mapa.inimigo[i].x + 1;
		
			if(Acertou(mapa, yTiro, xTiro, yInimigo, xInimigo)){
				//atribuicao das informacoes sobre a morte do inimigo
				mapa.inimigo[i].vida = 0;
				mapa.tiro.ativo = 0;
				mapa.inimigo[i].ordemM = mapa.ordem;
				mapa.ordem++;
				mapa.inimigo[i].iteracao = mapa.stats.interacoes;
				mapa.inimigo[i].yMorte = yTiro;
				mapa.inimigo[i].xMorte = xTiro;
				fprintf(ArquivoResumo, "[%d] Inimigo de indice %d da fileira %d foi atingido na posicao (%d %d).\n", mapa.stats.interacoes, mapa.inimigo[i].indice, mapa.inimigo[i].fileira, mapa.inimigo[i].xMorte -1, mapa.inimigo[i].yMorte -1);
				
				//ajuste do calculo de pontuacao
				mapa.stats.pontos += ((mapa.linhas - 2 )- mapa.inimigo[i].y) * mapa.inimigo[i].x;
			}
		}
	}
	
	return mapa;
}

int JogadorVenceu(tMapa mapa){
	int i, contador = 0;
	
	if(mapa.numInimigos > 0){
		
		for(i=0; i < mapa.numInimigos; i++){
			if(mapa.inimigo[i].vida == 1){
				contador++;
			}
		}
	}
	
	if(contador == 0){
		return 1;
	} else {
		return 0;
	}
		
}

int EstaCrescente(tRastreio rastreio[], int tam){
	int i, contador=0;
	tRastreio anterior;
	
	for(i=1; i < tam; i++){
		anterior = rastreio[i-1];
		if(anterior.linha > rastreio[i].linha){
			contador++;
		} else if(rastreio[i].linha == anterior.linha){
			if (anterior.ordem > rastreio[i].ordem){
				contador++;
			}
		}
	}
	
	if(contador == 0){
		return 1;
	} else {
		return 0;
	}
	
	
}

void OrdenaCrescenteLinhas(tRastreio rastreio[], int tam){
	tRastreio bau;
	tRastreio anterior;
	int i;
	
	while(!EstaCrescente(rastreio, tam)){
		for(i=1; i < tam; i++){
			anterior = rastreio[i-1];
		
			if(rastreio[i].linha == anterior.linha){
				if(anterior.ordem > rastreio[i].ordem){
					bau = anterior;
					anterior = rastreio[i];
					rastreio[i] = bau;
					rastreio[i-1] = anterior;
				}
			} else if (anterior.linha > rastreio[i].linha){
					bau = anterior;
					anterior = rastreio[i];
					rastreio[i] = bau;
					rastreio[i-1] = anterior;
			}
		}
	}
}

void AbreArqRanking(tMapa mapa, char diretorio[]){
	FILE * ArquivoRanking;
	char caminhoRanking[1000];
	
	tRastreio rastreio[mapa.numInimigos];
	
	//confeccao da entrada
	sprintf(caminhoRanking, "%s/saida/ranking.txt", diretorio);
	
	//cria o arquivo de ranking
	ArquivoRanking = fopen(caminhoRanking, "w");
	
	if(!ArquivoRanking){
		printf("ERRO: nao foi possivel criar o arquivo de ranking\n");
		exit(0);
	}
	
	//inicio do arquivo
	fprintf(ArquivoRanking, "indice,fileira,linha,iteracao\n");
	
	//atribui as linhas para ordenar
	int i;
	for(i=0; i < mapa.numInimigos; i++){
		if(mapa.inimigo[i].vida < 1){
			rastreio[i].linha = ((mapa.linhas)- mapa.inimigo[i].yMorte);
			rastreio[i].ordem = mapa.inimigo[i].ordemM;
		}
	}
	//ordena elas
	OrdenaCrescenteLinhas(rastreio, mapa.numInimigos);
	
	//printa o ranking dos inimigos
	int j, linha=0, ordemM=0;
	for(i=0; i < mapa.numInimigos; i++){
		for(j=0; j < mapa.numInimigos; j++){
			if(mapa.inimigo[j].vida < 1){
				
				linha = ((mapa.linhas)- mapa.inimigo[j].yMorte);
				ordemM = mapa.inimigo[j].ordemM;
				
				if((linha == rastreio[i].linha) && (ordemM == rastreio[i].ordem)){
					
					fprintf(ArquivoRanking, "%d,%d,%d,%d\n",mapa.inimigo[j].indice, mapa.inimigo[j].fileira, linha, mapa.inimigo[j].iteracao);
					
				}
			}
		}
	}
	fclose(ArquivoRanking);

	

}

void GeraArqEstatisticas (tMapa mapa, char diretorio[]){
	FILE * ArquivoEstatisticas;
	char caminhoEstatisticas[1000];
	
	//confeccao da entrada
	sprintf(caminhoEstatisticas, "%s/saida/estatisticas.txt", diretorio);
	
	//cria o arquivo de ranking
	ArquivoEstatisticas = fopen(caminhoEstatisticas, "w");
	
	if(!ArquivoEstatisticas){
		printf("ERRO: nao foi possivel criar o arquivo de estatisticas\n");
		exit(0);
	}
	
	fprintf(ArquivoEstatisticas, "Numero total de movimentos (A ou D): %d;\n", mapa.stats.qtdMovimento);
	fprintf(ArquivoEstatisticas, "Numero de tiros efetivos: %d;\n", mapa.stats.tirosEfetivos);
	fprintf(ArquivoEstatisticas, "Numero de tiros que nao acertaram: %d;\n", mapa.stats.tirosErrados);
	fprintf(ArquivoEstatisticas, "Numero de descidas dos inimigos: %d;\n", mapa.stats.descidaInimiga);
	
	fclose(ArquivoEstatisticas);

}

FILE * CriaArqResumo(char diretorio[]){
	FILE * ArquivoResumo;
	
	char caminhoResumo[1000];
	
	//confeccao do caminho para o resumo
	sprintf(caminhoResumo, "%s/saida/resumo.txt", diretorio);
	
	//acesso ao arquivo de resumo
	ArquivoResumo = fopen(caminhoResumo, "w");
	
	return ArquivoResumo;
}

void GeraArqHeatMap(tMapa mapa, char diretorio[]){
	FILE * ArquivoHeat;
	
	char caminhoHeat[1000];
	
	//confeccao do caminho para o resumo
	sprintf(caminhoHeat, "%s/saida/heatmap.txt", diretorio);
	
	//acesso ao arquivo de resumo
	ArquivoHeat = fopen(caminhoHeat, "w");
	
	if(!ArquivoHeat){
		printf("ERRO: falha ao criar o arquivo de mapa de calor.\n");
		exit(0);
	}
	
	int i, j;
	for(i=2; i < mapa.linhas; i++){
		for(j=2; j < mapa.colunas; j++){
			if(mapa.heatMap[i][j] > 999){
				 mapa.heatMap[i][j] = 999;
			}
			fprintf(ArquivoHeat, "%3d ", mapa.heatMap[i][j]);
		}
		fprintf(ArquivoHeat, "\n");
	}
	fclose(ArquivoHeat);
}

tMapa AdicionaJogadorNoCalor(tMapa mapa){
	
	int x = mapa.jogador.x;
	int y = mapa.jogador.y;
	
	//centro jogador
	mapa.heatMap[y][x] += 1;
	
	mapa.heatMap[y + 1][x] += 1;
	mapa.heatMap[y + 2][x] += 1;
	mapa.heatMap[y + 1][x + 1] += 1;
	mapa.heatMap[y + 1][x + 2] += 1;
	mapa.heatMap[y + 2][x + 1] += 1;
	mapa.heatMap[y + 2][x + 2] += 1;
	mapa.heatMap[y][x + 2] += 1;
	mapa.heatMap[y][x + 1] += 1;

	return mapa;
}

tMapa LeArqJogadas(tMapa mapa, char diretorio[]){
	FILE * ArquivoEntrada;
	FILE * ArquivoResumo;
	
	char jogada;

	
	char caminhoEntrada[1000];
	
	//confeccao do caminho para a Entrada
	sprintf(caminhoEntrada, "%s/entrada.txt", diretorio);
	
	//acesso ao arquivo de Entrada
	ArquivoEntrada = fopen(caminhoEntrada, "r");
	
	if(!ArquivoEntrada){
		printf("ERRO: nao foi possivel abrir o arquivo de entrada\n");
		exit(0);
	}
	
	ArquivoResumo = CriaArqResumo(diretorio);
	
	//Analisa os dados de entrada e executa
	while(!feof(ArquivoEntrada)){
		
		//adiciona a posicao atual do jogador no mapa de calor
		mapa = AdicionaJogadorNoCalor(mapa);

		//analisa se o jogador venceu
		if(JogadorVenceu(mapa)){
			printf("Pontos: %d | Iteracoes: %d\n", mapa.stats.pontos, mapa.stats.interacoes);
			MostraMapaJogo(mapa);
			printf("Parabéns, você ganhou!\n");
			break;
		}
		
		//analisa se os inimigos venceram
		if(InimigosNoFim(mapa)){
			printf("Pontos: %d | Iteracoes: %d\n", mapa.stats.pontos, mapa.stats.interacoes);
			MostraMapaJogo(mapa);
			printf("Você perdeu, tente novamente!\n");
			break;
		}
		
		//placar
		printf("Pontos: %d | Iteracoes: %d\n", mapa.stats.pontos, mapa.stats.interacoes);
		
		//printa o mapa
		MostraMapaJogo(mapa);
		
		//reseta o enderecao da animacao
		if(mapa.qualFormato == 3){
			mapa.qualFormato = 0;
		}
		
		//define a animacao
		if(mapa.bonus == 1){
			mapa.qualFormato++;
		}
		
		//verifica se te algum acerto
		mapa = TiroAcertou(mapa, ArquivoResumo);
		
		//passa a rodada
		mapa.stats.interacoes++;
		
		
		fscanf(ArquivoEntrada, "%c", &jogada);
		fscanf(ArquivoEntrada, "%*[^\n]");
		fscanf(ArquivoEntrada, "%*c");
		
		
		mapa = MovimentaTiro(mapa);
		
		mapa = MovimentaInimigos(mapa, ArquivoResumo); 
		
		mapa = EfetuaMovimentoJogador(mapa, jogada, ArquivoResumo);
		

	}
	fclose(ArquivoEntrada);
	
	//Abre o arquivo de ranking
	AbreArqRanking(mapa, diretorio);
	
	//Abre o arquivo de estatisticas
	GeraArqEstatisticas(mapa, diretorio);
	
	//Abre o arquivo de mapa de calor
	GeraArqHeatMap(mapa, diretorio);
	
	//Fecha o Resumo
	fclose(ArquivoResumo);
	
	
	return mapa;
}

tMapa ComecaJogo(tMapa mapa, char diretorio[]){
		//Roda o jogo por completo
		mapa = LeArqJogadas(mapa, diretorio);
		
		return mapa;
	
}