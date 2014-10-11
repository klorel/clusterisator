#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import itertools
# AMPL is useless for this ... so we do it with python
INSTANCES = {
	 1:'south'
	 ,
	 2:'SupremeCourtyes'
	 ,
	 3:'SupremeCourtnot'
	 ,
	 4:'SocialWorkJ'
	 ,
	 5:'wafa'
	 ,
	 6:'divorces'
	 ,
	 7:'hollyw'
	 ,
	 8:'scotl'
	 ,
	 9:'graphprod'
	 ,
	 10:'netscience'
	 ,
	11:'OR_costBurbridge'
	,
	12:'OR_costBurbridge'
	,
	13:'OR_costChan'
	,
	14:'OR_costGroover'
	,
	15:'OR_costKing'
	,
	16:'OR_costKKV'
	,
	17:'OR_costLee'
	,
	18:'OR_costMalakooti_a'
	,
	19:'OR_costMalakooti_b'
	,
	20:'OR_costMiltenburg'
}
FORMULATIONS = [
	"CPI"
	, 
	"CPI_varneg"
	,
	"CPS"
	,
	"CPS2"
]
 
def CreateInstance(instance):
	file = open('instance.txt', 'w')
	print>>file, '"'+INSTANCES[instance]+'"'
	file.close()

def CreateFormulation(formulation):
	file = open('formulation.txt', 'w')
	print>>file, '"'+formulation+'"'
	file.close()
	

def CreateOutput():
	file = open('result.csv', 'w')
	header=''
	header+= 'instance;'	
	header+= 'formulation;'	
	header+= 'n;'			
	header+= 'p;'			
	header+= 'm;'			
	header+= 'Nc;'			
	header+= 'Q;'			
	header+= 'real time(s);'
	header+= 'nb_nodes;'	
	header+= 'nb_vars;'		
	header+= 'nb_ctrs;'		
	header+= 'violations;'	
	header+= 'return code;'	
	print>>file, header
	# 'id;instance;n;p;m;Nc;Q;Nodes;Time;Model;Violations;Vars;MaxVars;Ctrs;MaxCtrs;Run;'
	file.close()

	
CreateOutput()
for (instance, formulation) in itertools.product(INSTANCES, FORMULATIONS):
	print instance, INSTANCES[instance]
	CreateInstance(instance)
	CreateFormulation(formulation)
	os.system( 'ampl clique.run' )
	# os.system( 'ampl clique2.run' )
	
