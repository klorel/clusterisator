#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import itertools
# AMPL is useless for this ... so we do it with python
BIPARTITE_INSTANCES = {
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

UNIPARTITE_INSTANCES = {
	1:'karate'
	,
	2:'dolphins'
	,
	3:'lesmis'
	,
	4:'celegans_metabolic'
	,
	5:'polbooks'
	,
	6:'football'
	,
	7:'adjnoun'
	,
	8:'celegansneural'
	,
	9:'jazz'
	,
	10:'netscience'
}

FORMULATIONS = [
#	"CPI"
#	, 
#	"CPI_varneg"
#	,
	"CPS2"
]
 
def CreateInstance(instance):
	file = open('instance.txt', 'w')
	print>>file, '"'+instance+'"'
	file.close()

def CreateFormulation(formulation):
	file = open('formulation.txt', 'w')
	print>>file, '"'+formulation+'"'
	file.close()
def CreateCriterion(criterion):
	file = open('criterion.txt', 'w')
	print>>file, '"'+criterion+'"'
	file.close()
	

def CreateOutput():
	file = open('result.csv', 'w')
	header='instance;formulation;n;p;m;Nc;Q;Time;nb_nodes;nb_vars;nb_ctrs;nb_violation;return_code;'
	output='';
	for column in header.split(';'):
		output+=column+';'
	print>>file, header
	# 
	file.close()

def LaunchBipartite():
	for (instance, formulation) in itertools.product(BIPARTITE_INSTANCES, FORMULATIONS):
		if instance==4:
			print instance, BIPARTITE_INSTANCES[instance]
			CreateInstance(BIPARTITE_INSTANCES[instance])
			CreateFormulation(formulation)
			CreateCriterion('BIPARTITE')
			os.system( 'ampl problem.run' )

def LaunchUnipartite():
	for (instance, formulation) in itertools.product(UNIPARTITE_INSTANCES, FORMULATIONS):
		if instance==5:
			print instance, UNIPARTITE_INSTANCES[instance]
			CreateInstance(UNIPARTITE_INSTANCES[instance])
			CreateFormulation(formulation)
			CreateCriterion('UNIPARTITE')
			os.system( 'ampl problem.run' )

CreateOutput()
#LaunchBipartite()
LaunchUnipartite()
	
