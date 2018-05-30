#include "Entity.h"

int checkSign(int tmp) {
    if(tmp>0) return 1;
    else if(tmp<0) return -1;
    else return 0;
}

int Entity::getDimension() {
    return this->dimension;
}

int Entity::getSpotStatus(int ii, int jj) {
    if(ii>=0 && jj>=0 && ii<this->dimension && jj<this->dimension) return board[ii][jj];
    else {
        //		cout<<"poza zakresem planszy"<<endl;
        return 0;
    }
}

void Entity::setSpotStatus(int ii, int jj, int status) {
    if(ii>=0 && jj>=0 && ii<this->dimension && jj<this->dimension && status<3 && status>-3) board[ii][jj]=status;
    else if (status<3 && status>-3) cout<<"poza zakresem planszy"<<endl;
    else cout<<"nie znam figury"<<endl;
}

void Entity::getBoard() {
    for(int ii=getDimension()-1; ii>=0; ii--) {
        cout<<ii+1<<"  ";
        for(int jj=0; jj<getDimension(); jj++) {
            if (getSpotStatus(jj,ii)>=0) cout<<" "<<getSpotStatus(jj,ii);
            else cout<<getSpotStatus(jj,ii);
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"    1 2 3 4 5 6 7 8"<<endl;
}

Entity::Entity(std::string name) {
    this->name = name;
    this->currentPlayer=1;
    this->dimension=dim;
    board = new int* [dim];
    for(int ii=0; ii<this->dimension; ii++) board[ii]= new int[dim];
    for(int ii=0; ii<this->dimension; ii++) {
        for(int jj=0; jj<this->dimension; jj++) {
            board[ii][jj]=0;
        }
    }


    for(int ii=0; ii<this->dimension; ii+=2) {
        board[ii][0]=1;
        board[ii+1][1]=1;
        board[ii][2]=1;

        board[ii+1][5]=-1;
        board[ii][6]=-1;
        board[ii+1][7]=-1;
    }
}

Entity::~Entity() {
    for(int ii=0; ii<dimension; ii++) delete [] board[ii];
    delete [] *board;
}


int Entity::getCurrentPlayer() {
    return this->currentPlayer;
}

void Entity::changeCurrentPlayer() {
    this->currentPlayer*=(-1);
    for(int ii=0; ii<this->dimension; ii++) {
        if (board[ii][0]==-1) board[ii][0]=-2;
        if (board[ii][7]== 1) board[ii][7]= 2;
    }
}

void Entity::info() {
    if(getCurrentPlayer() ==1 ) cout<<"tura bialego"<<endl;
    else if(getCurrentPlayer() ==-1 ) cout<<"tura czarnego"<<endl;
    getBoard();
}

bool Entity::scoreBoard() {
    int black=0;
    int white=0;
    for(int ii=0; ii<this->dimension; ii++) {
        for(int jj=0; jj<this->dimension; jj++) {
            if(board[ii][jj]>0) white++;
            else if	(board[ii][jj]<0) black++;
        }
    }
    if(black && white) {
        //cout<<"Na polu bitwy zostalo "<<white<< " bialych, oraz "<<black<<" czarnych."<<endl;
        return true;
    }
    else {
        if(white) cout<<"zwyciezyl bialy!"<<endl;
        else if(black) cout<<"zwyciezyl czarny!"<<endl;

        return false;
    }
}

bool Entity::checkDimension(int x1, int y1) {
    if(x1>=0 && y1>=0 && x1<this->dimension && y1<this->dimension) return true;
    else {
        //		cout<<"ruch poza plansze"<<endl;
        return false;
    }
}

bool Entity::checkRange(int x1, int y1,int x2, int y2) {
    if(abs(x2-x1)==1 && abs(y2-y1)==1) return true;
    else {
        cout<<"pion nie jest w zasiegu ruchu"<<endl;
        return false;
    }
}


bool Entity::checkBeatingWhite() {
    int x1=0,y1=0;
    for(int ii=0; ii<this->dimension; ii++) {
        for(int jj=0; jj<this->dimension; jj++) {
            x1=ii;
            y1=jj;
            if(checkDimension(x1,y1)) {
                if(getSpotStatus(x1,y1)==1) {
                    if(checkDimension(x1+1,y1+1) && checkDimension((x1+2), (y1+2))) {
                        if(getSpotStatus(x1,y1)*getSpotStatus(x1+1,y1+1)<0 && getSpotStatus( (x1+2), (y1+2) ) == 0 ) {
                            cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                            return true;
                        }
                    }
                    if(checkDimension(x1+1,y1-1) && checkDimension((x1+2), (y1-2))) {
                        if(getSpotStatus(x1,y1)*getSpotStatus(x1+1,y1-1)<0 && getSpotStatus( (x1+2), (y1-2) ) == 0 ) {
                            cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                            return true;
                        }
                    }
                    if(checkDimension(x1-1,y1+1) && checkDimension((x1-2), (y1+2))) {
                        if(getSpotStatus(x1,y1)*getSpotStatus(x1-1,y1+1)<0 && getSpotStatus( (x1-2), (y1+2) ) == 0 ) {
                            cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                            return true;
                        }
                    }
                    if(checkDimension(x1-1,y1-1) && checkDimension((x1-2), (y1-2))) {
                        if(getSpotStatus(x1,y1)*getSpotStatus(x1-1,y1-1)<0 && getSpotStatus( (x1-2), (y1-2) ) == 0 ) {
                            cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Entity::checkBeatingWhiteDame() {
    int x1=0,y1=0;
    for(int kk=0; kk<this->dimension; kk++) {
        for(int jj=0; jj<this->dimension; jj++) {
            x1=kk;
            y1=jj;
            if(checkDimension(x1,y1)) {
                if(getSpotStatus(x1,y1)==2) {
                    for(int ii=1; ii<7; ii++) {
                        if(checkDimension(x1+ii,y1+ii) && checkDimension((x1+ii+1), (y1+ii+1))) {
                            if(getSpotStatus(x1,y1)*getSpotStatus(x1+ii,y1+ii)<0 && getSpotStatus( (x1+ii+1), (y1+ii+1) ) == 0 ) {
                                cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                                return true;
                            }
                        }
                        if(checkDimension(x1+ii,y1-ii) && checkDimension((x1+ii+1), (y1-ii-1))) {
                            if(getSpotStatus(x1,y1)*getSpotStatus(x1+ii,y1-ii)<0 && getSpotStatus( (x1+ii+1), (y1-ii-1) ) == 0 ) {
                                cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                                return true;
                            }
                        }
                        if(checkDimension(x1-ii,y1+ii) && checkDimension((x1-ii-1), (y1+ii+1))) {
                            if(getSpotStatus(x1,y1)*getSpotStatus(x1-ii,y1+ii)<0 && getSpotStatus( (x1-ii-1), (y1+ii+1) ) == 0 ) {
                                cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                                return true;
                            }
                        }
                        if(checkDimension(x1-ii,y1-ii) && checkDimension((x1-ii-1), (y1-ii-1))) {
                            if(getSpotStatus(x1,y1)*getSpotStatus(x1-ii,y1-ii)<0 && getSpotStatus( (x1-ii-1), (y1-ii-1) ) == 0 ) {
                                cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Entity::checkBeatingBlack() {
    int x1=0,y1=0;
    for(int ii=0; ii<this->dimension; ii++) {
        for(int jj=0; jj<this->dimension; jj++) {
            x1=ii;
            y1=jj;
            if(checkDimension(x1,y1)) {
                if(getSpotStatus(x1,y1)==-1) {
                    if(checkDimension(x1+1,y1+1) && checkDimension((x1+2), (y1+2))) {
                        if(getSpotStatus(x1,y1)*getSpotStatus(x1+1,y1+1)<0 && getSpotStatus( (x1+2), (y1+2) ) == 0 ) {
                            cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                            return true;
                        }
                    }
                    if(checkDimension(x1+1,y1-1) && checkDimension((x1+2), (y1-2))) {
                        if(getSpotStatus(x1,y1)*getSpotStatus(x1+1,y1-1)<0 && getSpotStatus( (x1+2), (y1-2) ) == 0 ) {
                            cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                            return true;
                        }
                    }
                    if(checkDimension(x1-1,y1+1) && checkDimension((x1-2), (y1+2))) {
                        if(getSpotStatus(x1,y1)*getSpotStatus(x1-1,y1+1)<0 && getSpotStatus( (x1-2), (y1+2) ) == 0 ) {
                            cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                            return true;
                        }
                    }
                    if(checkDimension(x1-1,y1-1) && checkDimension((x1-2), (y1-2))) {
                        if(getSpotStatus(x1,y1)*getSpotStatus(x1-1,y1-1)<0 && getSpotStatus( (x1-2), (y1-2) ) == 0 ) {
                            cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Entity::checkBeatingBlackDame() {
    int x1=0,y1=0;
    for(int kk=0; kk<this->dimension; kk++) {
        for(int jj=0; jj<this->dimension; jj++) {
            x1=kk;
            y1=jj;
            if(checkDimension(x1,y1)) {
                if(getSpotStatus(x1,y1)==-2) {
                    for(int ii=1; ii<7; ii++) {
                        if(checkDimension(x1+ii,y1+ii) && checkDimension((x1+ii+1), (y1+ii+1))) {
                            if(getSpotStatus(x1,y1)*getSpotStatus(x1+ii,y1+ii)<0 && getSpotStatus( (x1+ii+1), (y1+ii+1) ) == 0 ) {
                                cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                                return true;
                            }
                        }
                        if(checkDimension(x1+ii,y1-ii) && checkDimension((x1+ii+1), (y1-ii-1))) {
                            if(getSpotStatus(x1,y1)*getSpotStatus(x1+ii,y1-ii)<0 && getSpotStatus( (x1+ii+1), (y1-ii-1) ) == 0 ) {
                                cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                                return true;
                            }
                        }
                        if(checkDimension(x1-ii,y1+ii) && checkDimension((x1-ii-1), (y1+ii+1))) {
                            if(getSpotStatus(x1,y1)*getSpotStatus(x1-ii,y1+ii)<0 && getSpotStatus( (x1-ii-1), (y1+ii+1) ) == 0 ) {
                                cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                                return true;
                            }
                        }
                        if(checkDimension(x1-ii,y1-ii) && checkDimension((x1-ii-1), (y1-ii-1))) {
                            if(getSpotStatus(x1,y1)*getSpotStatus(x1-ii,y1-ii)<0 && getSpotStatus( (x1-ii-1), (y1-ii-1) ) == 0 ) {
                                cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}


bool Entity::checkBeatingWhite(int x1, int y1) {
    if(checkDimension(x1,y1)) {
        if(getSpotStatus(x1,y1)==1) {
            if(checkDimension(x1+1,y1+1) && checkDimension((x1+2), (y1+2))) {
                if(getSpotStatus(x1,y1)*getSpotStatus(x1+1,y1+1)<0 && getSpotStatus( (x1+2), (y1+2) ) == 0 ) {
                    cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                    return true;
                }
            }
            if(checkDimension(x1+1,y1-1) && checkDimension((x1+2), (y1-2))) {
                if(getSpotStatus(x1,y1)*getSpotStatus(x1+1,y1-1)<0 && getSpotStatus( (x1+2), (y1-2) ) == 0 ) {
                    cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                    return true;
                }
            }
            if(checkDimension(x1-1,y1+1) && checkDimension((x1-2), (y1+2))) {
                if(getSpotStatus(x1,y1)*getSpotStatus(x1-1,y1+1)<0 && getSpotStatus( (x1-2), (y1+2) ) == 0 ) {
                    cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                    return true;
                }
            }
            if(checkDimension(x1-1,y1-1) && checkDimension((x1-2), (y1-2))) {
                if(getSpotStatus(x1,y1)*getSpotStatus(x1-1,y1-1)<0 && getSpotStatus( (x1-2), (y1-2) ) == 0 ) {
                    cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Entity::checkBeatingWhiteDame(int x1, int y1) {
    if(checkDimension(x1,y1)) {
        if(getSpotStatus(x1,y1)==2) {
            for(int ii=1; ii<7; ii++) {
                if(checkDimension(x1+ii,y1+ii) && checkDimension((x1+ii+1), (y1+ii+1))) {
                    if(getSpotStatus(x1,y1)*getSpotStatus(x1+ii,y1+ii)<0 && getSpotStatus( (x1+ii+1), (y1+ii+1) ) == 0 ) {
                        cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                        return true;
                    }
                }
                if(checkDimension(x1+ii,y1-ii) && checkDimension((x1+ii+1), (y1-ii-1))) {
                    if(getSpotStatus(x1,y1)*getSpotStatus(x1+ii,y1-ii)<0 && getSpotStatus( (x1+ii+1), (y1-ii-1) ) == 0 ) {
                        cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                        return true;
                    }
                }
                if(checkDimension(x1-ii,y1+ii) && checkDimension((x1-ii-1), (y1+ii+1))) {
                    if(getSpotStatus(x1,y1)*getSpotStatus(x1-ii,y1+ii)<0 && getSpotStatus( (x1-ii-1), (y1+ii+1) ) == 0 ) {
                        cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                        return true;
                    }
                }
                if(checkDimension(x1-ii,y1-ii) && checkDimension((x1-ii-1), (y1-ii-1))) {
                    if(getSpotStatus(x1,y1)*getSpotStatus(x1-ii,y1-ii)<0 && getSpotStatus( (x1-ii-1), (y1-ii-1) ) == 0 ) {
                        cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Entity::checkBeatingBlack(int x1, int y1) {
    if(checkDimension(x1,y1)) {
        if(getSpotStatus(x1,y1)==-1) {
            if(checkDimension(x1+1,y1+1) && checkDimension((x1+2), (y1+2))) {
                if(getSpotStatus(x1,y1)*getSpotStatus(x1+1,y1+1)<0 && getSpotStatus( (x1+2), (y1+2) ) == 0 ) {
                    cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                    return true;
                }
            }
            if(checkDimension(x1+1,y1-1) && checkDimension((x1+2), (y1-2))) {
                if(getSpotStatus(x1,y1)*getSpotStatus(x1+1,y1-1)<0 && getSpotStatus( (x1+2), (y1-2) ) == 0 ) {
                    cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                    return true;
                }
            }
            if(checkDimension(x1-1,y1+1) && checkDimension((x1-2), (y1+2))) {
                if(getSpotStatus(x1,y1)*getSpotStatus(x1-1,y1+1)<0 && getSpotStatus( (x1-2), (y1+2) ) == 0 ) {
                    cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                    return true;
                }
            }
            if(checkDimension(x1-1,y1-1) && checkDimension((x1-2), (y1-2))) {
                if(getSpotStatus(x1,y1)*getSpotStatus(x1-1,y1-1)<0 && getSpotStatus( (x1-2), (y1-2) ) == 0 ) {
                    cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Entity::addPlayer(int id) {
	int size = players.size();
    if(size>1) {
		return false;
    } else{
		players.push_back(id);
		return true;
    }
}

bool Entity::checkBeatingBlackDame(int x1, int y1) {
    if(checkDimension(x1,y1)) {
        if(getSpotStatus(x1,y1)==-2) {
            for(int ii=1; ii<7; ii++) {
                if(checkDimension(x1+ii,y1+ii) && checkDimension((x1+ii+1), (y1+ii+1))) {
                    if(getSpotStatus(x1,y1)*getSpotStatus(x1+ii,y1+ii)<0 && getSpotStatus( (x1+ii+1), (y1+ii+1) ) == 0 ) {
                        cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                        return true;
                    }
                }
                if(checkDimension(x1+ii,y1-ii) && checkDimension((x1+ii+1), (y1-ii-1))) {
                    if(getSpotStatus(x1,y1)*getSpotStatus(x1+ii,y1-ii)<0 && getSpotStatus( (x1+ii+1), (y1-ii-1) ) == 0 ) {
                        cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                        return true;
                    }
                }
                if(checkDimension(x1-ii,y1+ii) && checkDimension((x1-ii-1), (y1+ii+1))) {
                    if(getSpotStatus(x1,y1)*getSpotStatus(x1-ii,y1+ii)<0 && getSpotStatus( (x1-ii-1), (y1+ii+1) ) == 0 ) {
                        cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                        return true;
                    }
                }
                if(checkDimension(x1-ii,y1-ii) && checkDimension((x1-ii-1), (y1-ii-1))) {
                    if(getSpotStatus(x1,y1)*getSpotStatus(x1-ii,y1-ii)<0 && getSpotStatus( (x1-ii-1), (y1-ii-1) ) == 0 ) {
                        cout<<"jest mozliwe bicie - musisz je wykonac"<<endl;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


bool Entity::checkDimension(int x1, int y1,int x2, int y2) {
    if(x1>=0 && y1>=0 && x1<this->dimension && y1<this->dimension && x2>=0 && y2>=0 && x2<this->dimension && y2<this->dimension) return true;
    else return false;
}

bool Entity::checkMoveWhite(int x1, int y1,int x2, int y2) {
    if(checkDimension(x1,y1,x2,y2)) {	//zasieg tablicy
        if (getSpotStatus(x1,y1)==1) {	// czy to bialy pionek
            if(checkRange(x1,y1,x2,y2)) {	// czy pionek bedzie mial zasieg
                if(getSpotStatus(x2,y2)==0 && ( y2-y1 ) == 1 ) {	//czy wolne miejsce i nie rusza sie do tylu
                    if(!( checkBeatingWhite() || checkBeatingWhiteDame() )) {	//czy mozliwe jest wykonanie bicia
                        setSpotStatus(x2,y2,getSpotStatus(x1,y1));
                        setSpotStatus(x1,y1,0);
                        return true;
                    } else return false;
                } else if (getSpotStatus(x2,y2)<0 && checkDimension( (x2+(x2-x1)), y2+(y2-y1)) ) {	//czy na tym miejscu jest przeciwnik (wowczas moze sie ruszyc do tylu ewentualnie)
                    if ( getSpotStatus( x2+(x2-x1), y2+(y2-y1) ) == 0) {	//
                        setSpotStatus(  x2+(x2-x1), y2+(y2-y1),getSpotStatus(x1,y1));
                        setSpotStatus(x1,y1,0);
                        setSpotStatus(x2,y2,0);
                        if(! checkBeatingWhite( x2+(x2-x1),y2+(y2-y1) )) {	//czy mozliwe jest wykonanie jeszcze dodatkowego bicia
                            return true;
                        } else return false;
                    } else return false;
                } else return false;
            } else return false;
        } else return false;
    } else return false;
}

bool Entity::checkMoveWhiteDame(int x1, int y1,int x2, int y2) {
    if(checkDimension(x1,y1,x2,y2)) {	//zasieg tablicy
        if (getSpotStatus(x1,y1)==2) {	// czy to biala dama
            if(abs(x2-x1)==abs(y2-y1)) {	// czy dama porusza sie po przekatnej
                if(getSpotStatus(x2,y2)==0) {	//czy wolne miejsce
                    for(int ii=1; ii<=abs(x2-x1); ii++) if((getSpotStatus(x1+(ii*checkSign(x2-x1)), y1+(ii*checkSign(y2-y1)) )>0)) return false;	//czy po drodze nie ma bialych
                    int tmp=0;
                    for(int ii=1; ii<=abs(x2-x1); ii++) tmp+=( getSpotStatus(x1+(ii*checkSign(x2-x1)), y1+(ii*checkSign(y2-y1)) ) -1)/2;			// czy po drodze napotkal 0, 1 ,lub wiecj czarnych
                    if(tmp==0) {	//0 czarnych
                        if(!( checkBeatingWhite() || checkBeatingWhiteDame() )) {	//czy mozliwe jest wykonanie bicia
                            setSpotStatus(x2,y2,getSpotStatus(x1,y1));
                            setSpotStatus(x1,y1,0);
                            return true;
                        } else return false;
                    } else if (abs(tmp)==1) {	// 1 czarny
                        setSpotStatus(x2,y2,getSpotStatus(x1,y1));
                        for(int ii=0; ii<abs(x2-x1); ii++) setSpotStatus( x1+(ii*checkSign(x2-x1)), y1+(ii*checkSign(y2-y1)), 0) ;
                        if( ! checkBeatingWhiteDame(x2,y2) ) {	//czy mozliwe jest wykonanie jeszcze dodatkowego bicia
                            return true;
                        } else return false;
                    } else return false;
                } else return false;
            } else return false;
        } else return false;
    } else return false;
}


bool Entity::checkMoveBlack(int x1, int y1,int x2, int y2) {
    if(checkDimension(x1,y1,x2,y2)) {	//zasieg tablicy
        if (getSpotStatus(x1,y1)<0) {	// czy to czarny pionek
            if(checkRange(x1,y1,x2,y2)) {	// czy pionek bedzie mial zasieg
                if(getSpotStatus(x2,y2)==0 && ( y2-y1 ) == -1 ) {	//czy wolne miejsce i nie rusza sie do tylu
                    if(! ( checkBeatingBlack() || checkBeatingBlackDame() ) ) {	//czy mozliwe jest wykonanie bicia
                        setSpotStatus(x2,y2,getSpotStatus(x1,y1));
                        setSpotStatus(x1,y1,0);
                        return true;
                    } else return false;
                } else if (getSpotStatus(x2,y2)>0 && checkDimension( (x2+(x2-x1)), y2+(y2-y1)) ) {	//czy na tym miejscu jest przeciwnik (wowczas moze sie ruszyc do tylu ewentualnie)
                    if ( getSpotStatus( (x2+(x2-x1)), y2+(y2-y1) ) == 0) {	//
                        setSpotStatus( (x2+(x2-x1)), y2+(y2-y1),getSpotStatus(x1,y1));
                        setSpotStatus(x1,y1,0);
                        setSpotStatus(x2,y2,0);
                        if(! checkBeatingWhite(x2+(x2-x1),y2+(y2-y1))) {	//czy mozliwe jest wykonanie jeszcze dodatkowego bicia
                            return true;
                        } else return false;
                    } else return false;
                } else return false;
            } else return false;
        } else return false;
    } else return false;
}

bool Entity::checkMoveBlackDame(int x1, int y1,int x2, int y2) {
    if(checkDimension(x1,y1,x2,y2)) {	//zasieg tablicy
        if (getSpotStatus(x1,y1)==-2) {	// czy to czarna dama
            if(abs(x2-x1)==abs(y2-y1)) {	// czy dama porusza sie po przekatnej
                if(getSpotStatus(x2,y2)==0) {	//czy wolne miejsce
                    for(int ii=1; ii<=abs(x2-x1); ii++) if((getSpotStatus(x1+(ii*checkSign(x2-x1)), y1+(ii*checkSign(y2-y1)) )<0) ) return false;	//czy po drodze nie ma czarnych
                    int tmp=0;
                    for(int ii=1; ii<=abs(x2-x1); ii++) tmp+=( getSpotStatus(x1+(ii*checkSign(x2-x1)), y1+(ii*checkSign(y2-y1)) ) +1)/2;			// czy po drodze napotkal 0, 1 ,lub wiecj bialych
                    if(tmp==0) {	//0 bialych
                        if(! ( checkBeatingBlack() || checkBeatingBlackDame() ) ) {	//czy mozliwe jest wykonanie bicia
                            setSpotStatus(x2,y2,getSpotStatus(x1,y1));
                            setSpotStatus(x1,y1,0);
                            return true;
                        } else return false;
                    } else if (abs(tmp)==1) {	// 1 bialy
                        setSpotStatus(x2,y2,getSpotStatus(x1,y1));
                        for(int ii=0; ii<abs(x2-x1); ii++) setSpotStatus( x1+(ii*checkSign(x2-x1)), y1+(ii*checkSign(y2-y1)), 0) ;
                        if( ! checkBeatingBlackDame(x2,y2) ) {	//czy mozliwe jest wykonanie jeszcze dodatkowego bicia
                            return true;
                        } else return false;
                    } else return false;
                } else return false;
            } else return false;
        } else return false;
    } else return false;
}


bool Entity::checkMove(int x1, int y1,int x2, int y2) {
    if (getCurrentPlayer()==1 && getSpotStatus(x1,y1)==1) return checkMoveWhite(x1,y1,x2,y2);
    else if (getCurrentPlayer()==1 && getSpotStatus(x1,y1)==2) return checkMoveWhiteDame(x1,y1,x2,y2);
    else if (getCurrentPlayer()==-1 && getSpotStatus(x1,y1)==-1) return checkMoveBlack(x1,y1,x2,y2);
    else if (getCurrentPlayer()==-1 && getSpotStatus(x1,y1)==-2) return checkMoveBlackDame(x1,y1,x2,y2);
    else return false;

}
