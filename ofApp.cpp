#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	ofSetVerticalSync();
		true
			60fps
			
		false
			trueの10倍くらい出る
	********************/
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	
	/********************
	********************/
	ofVec3f point[NUM_POINTS];
	const float size = 100;
	point[0] = ofVec3f(-size,  size, 0);
	point[1] = ofVec3f(-size, -size, 0);
	point[2] = ofVec3f( size,  size, 0);
	point[3] = ofVec3f( size, -size, 0);
	
	for(int i = 0; i < NUM_POINTS; i++){
		Verts[i].set(point[i]);
		Color[i].set(0.0, 0.0, 1.0, 0.9);
	}
	
	Vbo.setVertexData(Verts, NUM_POINTS, GL_DYNAMIC_DRAW);
	Vbo.setColorData(Color, NUM_POINTS, GL_DYNAMIC_DRAW);
	
	/********************
	********************/
	gui.setup();
	
	gui.add(b_Cull.setup("cull", false));
	gui.add(b_Cull_Front.setup("b_Cull_Front", false));
	gui.add(b_FrontIsClockWise.setup("b_FrontIsClockWise", false));
	
}

//--------------------------------------------------------------
void ofApp::update(){
	/********************
	http://www.komoto.org/opengl/sample10.html
	
	ポリゴンの表裏は、処理軽くするために使われます。
	つまり、ポリゴンのおもて面のみを描き、裏を描かないようにすることで、描くポリゴンを減らし、処理を軽くします。
	使う関数は、次の２つです。
		glEnable(GL_CULL_FACE)			...片面表示を有効にします。
		void glCullFace(GLenum mode)	...どの面を描画しないようにするかを指定します。
		
	modeは、
		GL_FRONT
		GL_BACK
		GL_FRONT_AND_BACK
	のいずれかです。
	GL_FRONTを指定すると、表面を描画しない、つまり、裏面のみを描画します。
	GL_BACKなら、表面のみが描かれ、GL_FRONT_AND_BACKなら、ポリゴンは描かれなくなります。

  まとめると、次のようになります。
		両面を描く
			glDisable(GL_CULL_FACE);
			
		表面のみを描く
			glEnable(GL_CULL_FACE); 
			glCullFace(GL_BACK);
			
		裏面のみを描く
			glEnable(GL_CULL_FACE); 
			glCullFace(GL_FRONT);
	********************/
	
	/********************
	http://seesaawiki.jp/w/mikk_ni3_92/d/%B4%F0%CB%DC%CA%D427
		ポリゴンは頂点座標が与えられた時、反時計回りになっている側を「表」とする。
		どちらが表かを設定するのは、「glFrontFace関数」である。
		「GL_CCW」 (counter clockwise)を指定すると、反時計回りが表
		「GL_CW」(clockwise)を指定すると時計回りが表
		
		my note
			ofVboのpageで言う、GL_TRIANGLESで指定する方向が時計回り.
			方向が わからなくなっても、現物あわせでOK.
	********************/
	if(b_Cull){
		if(b_Cull_Front){
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}else{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		
		if(b_FrontIsClockWise)	glFrontFace(GL_CW);//時計回りが表
		else					glFrontFace(GL_CCW);//反時計回りが表
		
	}else{
		glDisable(GL_CULL_FACE);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	/********************
	********************/
	ofBackground(0);
	
	/********************
	Z-buffering and blemding mode do not work together properly in a simple way.
	(oF Essentials. p98)
	
	参考URL
		https://www.sjbaker.org/steve/omniv/alpha_sorting.html
	********************/
	// z buffering
	ofDisableAlphaBlending();
	ofEnableDepthTest();
	
	/*
	// Alpha
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	*/
	
	/********************
	********************/
	ofSetColor(255);
	
	/********************
	********************/
	cam.begin();
	
	for(int i = 0; i < NUM_POINTS; i++){
		Color[i].set(0.0, 0.0, 1.0, 0.9);
	}
	Vbo.updateColorData(Color, NUM_POINTS);
	Vbo.draw(GL_TRIANGLE_STRIP, 0, NUM_POINTS);
	
	/*
	ズラして描く。
	
	(1)先に描いた図が奥にある場合、
	1st object draw
	2nd object draw
		2ndは手前なので全て描画される
		裏に1stが見える
	結果
		translucent
		
	(2)先に描いた図が手前にある場合、
	1st object draw
	2nd object draw
		2ndは奥なので、隠れた部分は描画されない
		裏にある2ndは最初から描画されない
	結果
		opaque
	*/
	ofPushMatrix();
	ofTranslate(50, -50, -100);
	ofRotate(180, 0, 1, 0);
		for(int i = 0; i < NUM_POINTS; i++){
			Color[i].set(1.0, 0.0, 0.0, 0.9);
		}
		Vbo.updateColorData(Color, NUM_POINTS);
		Vbo.draw(GL_TRIANGLE_STRIP, 0, NUM_POINTS);
	ofPopMatrix();
	
	cam.end();
	
	/********************
	********************/
	clear_VboSetting_gl();
	
	/********************
	********************/
	glDisable(GL_CULL_FACE);	// for gui.draw();
	ofDisableDepthTest();		// for gui.draw();
	gui.draw();
	
	/*
	string info;
	info += "FPS = " + ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapString(info, 30, 30);
	*/
}

/******************************
descrition
	ofVboで描画すると、openGlの設定が何らか変わるようだ。
	この結果、次に来る描画が所望の動作とならないケース多数。
		次のfunctionが描画されないなど
			ofDrawBitmapString()
			image.draw()
			
	この対応として、
		ofCircle(). ofRect().
	等を1発いれてやることで、OKとなった。
	おそらく、この関数内で、openGl設定が、また変わるのだろう。
		α = 0;
	にて描画する。
******************************/
void ofApp::clear_VboSetting_gl()
{
	ofSetColor(255, 255, 255, 0);
	ofCircle(0, 0, 1);
	
	ofSetColor(255, 255, 255, 255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
