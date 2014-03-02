#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    
    camWidth = 1280;
    camHeight = 720;
    threshNum = 6;
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    
    vidGrabber.initGrabber(camWidth, camHeight);
    
    tex.setTextureWrap(GL_REPEAT, GL_REPEAT);
    tex.allocate(camWidth,camHeight,GL_RGB,GL_RGBA);
    tbIm.allocate(camWidth,camHeight,OF_IMAGE_COLOR_ALPHA);
    
    
    meshInit();
}

//--------------------------------------------------------------
void testApp::update(){
    
    imageUpdate();
}

//--------------------------------------------------------------
void testApp::draw()
{
    updateSpring();
    mesh.clearVertices();
    mesh.addVertices(vertexCoords);
    
    tex.bind();
    mesh.draw();
    //mesh.drawWireframe();
    tex.unbind();
    
    
}

//--------------------------------------------------------------
void testApp::imageUpdate()
{
    vidGrabber.update();
    if (vidGrabber.isFrameNew())
    {
        for(int j = 0; j < camWidth; j++)
        {
            for(int k = 0; k < camHeight; k++)
            {
                tbIm.setColor(j,k,vidGrabber.getPixelsRef().getColor(j, k));
            }
        }
        
        tbIm.reloadTexture();
        tex = tbIm.getTextureReference();
    }
}

//--------------------------------------------------------------
void testApp::meshInit()
{
    int cb = 7;
    coordX = cb;
    coordY = cb;
    
    distX = camWidth/coordX;
    distY = camHeight/coordY;
    
    float coordYf = (float) coordY;
    float coordXf = (float) coordX;
    float widthf = (float) camWidth;
    float heightf = (float) camHeight;
    coordLength = coordX * coordY;
    ratioCoordX = widthf / (coordXf-1.0);
    ratioCoordY = heightf / (coordYf-1.0);
    
    vertexCoords.resize(coordLength);
    texCoords.resize(coordLength);
    velocities.resize(coordLength);
    tempCoords.resize(coordLength);
    
    
    for(int y = 0; y < coordY; y++)
    {
        for(int x = 0; x < coordX; x++)
        {
            vertexCoords.at((y*(coordX))+x).set(ratioCoordX*x,ratioCoordY*y,0.0);
            texCoords.at((y*(coordX))+x).set(ratioCoordX*x,ratioCoordY*y);
            
            velocities.at((y*(coordX))+x).set(0,0);
        }
    }
    tempCoords = texCoords;
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    mesh.addTexCoords(texCoords);
    mesh.addVertices(vertexCoords);
    
    //ADD INDICIES TO MATRIX FOR TRIANGLE STRIP MESH DRAWING ORDER
    //code based on code from here
    //http://www.learnopengles.com/android-lesson-eight-an-introduction-to-index-buffer-objects-ibos/
    
    for(short y = 0; y < coordY-1; y++)
    {
        if( y > 0)
        {
            mesh.addIndex(y*coordY);
        }
        
        for(short x = 0; x < coordX; x++)
        {
            mesh.addIndex((y*coordY)+x);
            mesh.addIndex(((y+1)*coordY)+x);
        }
        if(y < coordY -2)
        {
            mesh.addIndex(((y+1)*coordY)+(coordX - 1));
        }
    }
    
    meshCopy = mesh;
}


//--------------------------------------------------------------
void testApp::updateSpring()
{
    for(int i = 1; i < coordY -1; i++)
    {
        for(int j = 1; j < coordX -1; j++)
        {
            int index = ((i*coordY)+j);
            tempCoords.at(index) += velocities.at(index);
            ofVec2f velTemp = velReturn(index);
            
            velocities.at(index) += velTemp;
            tempCoords.at(index) += velTemp;
            
            vertexCoords.at(index).set(tempCoords.at(index).x, tempCoords.at(index).y);
            
        }
    }
}

//--------------------------------------------------------------
ofVec2f testApp::velReturn(int index)
{
    ofVec2f velR, fLeft, fRight, fTop, fBottom;
    velR.set(0,0);
    
    fLeft = springVel(index, index-1, distX);
    fRight = springVel(index, index+1, distX);
    fTop = springVel(index, index-coordY, distY);
    fBottom = springVel(index, index+coordY, distY);
    
    velR = fLeft + fRight + fBottom + fTop;
    
    return velR;
}

//--------------------------------------------------------------
ofVec2f testApp::springVel(int index1, int index2, double rest)
{
    ofVec2f velR;
    float dist, x, force;
    dist = tempCoords.at(index1).distance(tempCoords.at(index2));
    x = dist - rest;
    force = - x/10000;
    velR = tempCoords.at(index1) - tempCoords.at(index2);
    velR *= force;
    return velR;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    /*
     ofVec2f theforce;
     theforce.set(100,0);
     for(int i = 0; i < coordLength; i++)
     {
     velocities.at(i) += theforce;
     }
     */
    
    tempCoords.at(floor(coordLength/2)).set(mouseX, mouseY);
    
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}