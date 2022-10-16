def applicability_condition(self,Agent):
    return SUCCESS

def preparatory_spec(self,Agent):
    return SUCCESS

def execution_steps(self,Agent):
    #Get all objects of the agnet, and put it in a list
    objects = []
    for i in range(numContent(Agent)):
        objects.append(getContent(Agent,i))
    createContents(objects)
    return {'PRIMITIVE':('Action',{'agents':Agent})}

def culmination_condition(self,Agent):
    if finishedAction(self.id):
        objects =[i for i in contents]
        printContents(objects)
	return SUCCESS
    return INCOMPLETE
