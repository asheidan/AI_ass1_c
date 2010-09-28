import re

env = Environment(tools=['default','cutest'],toolpath=['scons'])
env.Append(CFLAGS=['-g','-Wall','-D__DEBUG__'])
env.Append(CPPPATH=['.','cutest','libirk'])
env.Append(LIBPATH=['.'])
# env.Append(FRAMEWORKS=['Foundation'])

cutest = env.Object('cutest/CuTest.c')

libirk = env.Library('irk',['libirk/'+ f for f in ['ILHash.c','ILSort.c']])

irk_test_files = Glob('libirk/*Test.c')
if(len(irk_test_files) > 0):
	irk_test_suite = ['libirk/IrkTests.c']
	env.GenerateAllTests(irk_test_suite,irk_test_files)
	irk_tests = env.Program(irk_test_suite + irk_test_files + cutest,LIBS=['irk'])
	run_irk_tests = Command('libtest',irk_tests, './$SOURCE')
	# Default(run_irk_tests)

classes = ['Board.c','PlayerHuman.c','PlayerMinMax.c']
target = env.Program("game",['main.c'] + classes,LIBS=['c','irk'])
env.Command('run', target, './$SOURCE ../maps/day1.map')
Default(target)


test_files = Glob('*Test.c')
if(len(test_files) > 0):
	tested_files = [re.sub('Test.c','.o',f) for f in [str(s) for s in test_files]]
	test_suite = env.GenerateAllTests('AllTests.c',test_files)
	alltests = env.Program([test_suite] + test_files + tested_files + cutest,LIBS=['irk'])
	runtests = Command('test',alltests, './$SOURCE')
	#Default(runtests)

Depends(target,runtests)
# Depends(target,run_irk_tests)
