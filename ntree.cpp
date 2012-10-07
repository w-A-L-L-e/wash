/*=============================================================================
author        :Walter Schreppers
filename      :ntree.cpp
description   :Here we test the XMLNode class (it represents an n-airy tree)
bugreport(log):/
=============================================================================*/

#include "treenode.h"
#include <iostream>



int main(){
  TreeNode t;
  t.setName("root");

  TreeNode* child1=new TreeNode();
  child1->setName("child1");
  t.appendChild(child1);
  
  TreeNode* child2=new TreeNode();
  child2->setName("child2");
  t.appendChild(child2);
  
  
  TreeNode* child3=new TreeNode();
  child3->setName("child3");
  t.appendChild(child3);


  //4 kinderen van 2e child
  TreeNode* child4=new TreeNode();
  child4->setName("childchild1");
  child2->appendChild(child4);
  
  TreeNode* child5=new TreeNode();
  child5->setName("childchild2");
  child2->appendChild(child5);

  child5=new TreeNode();
  child5->setName("childchild3");
  child2->appendChild(child5);
  
  child5=new TreeNode();
  child5->setName("childchild4");
  child2->appendChild(child5);

  t.showTree(&t);


  cout<<endl<<"test nextSibling function"<<endl;
  TreeNode* t2=t.firstChild();
  while(t2!=0){
    cout<<t2->getName();
    if(t2->hasChildren()) cout<<" has children";
    cout<<endl; 
    t2=t2->nextSibling();
  }

  cout<<endl<<"test prevSibling function"<<endl;
  TreeNode* t3=t.lastChild();
  while(t3!=0){
    cout<<t3->getName();
    if(t3->hasChildren()) cout<<" has children";
    cout<<endl;
    t3=t3->prevSibling();
  }


  //test child functions
  cout<<endl<<"child functions"<<endl;
  cout<<t.firstChild()->getName()<<endl;  
  cout<<t.secondChild()->getName()<<endl;
  cout<<"\t"<<t.secondChild()->firstChild()->getName()<<endl;
  cout<<"\t"<<t.secondChild()->secondChild()->getName()<<endl;
  cout<<"\t"<<t.secondChild()->thirdChild()->getName()<<endl;
  cout<<t.thirdChild()->getName()<<endl;  
  
}


