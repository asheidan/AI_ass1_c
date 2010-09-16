import re

env = Environment(tools=['default','cutest'],toolpath=['scons'])
env.Append(CFLAGS=['-g','-Wall','-D__DEBUG__'])
env.Append(CPPPATH=['.','cutest','ilhash'])
env.Append(LIBPATH=['.'])
# env.Append(FRAMEWORKS=['Foundation'])

cutest = env.Object('cutest/CuTest.c')

hashlib = env.Library('hash',['ilhash/'+ f for f in ['ILHash.c']])

hash_test_files = Glob('ilhash/*Test.c')
if(len(hash_test_files) > 0):
	hash_test_suite = ['ilhash/HashTests.c']
	env.GenerateAllTests(hash_test_suite,hash_test_files)
	hash_tests = env.Program(hash_test_suite + hash_test_files + cutest,LIBS=['hash'])
	run_hash_tests = Command('hashtest',hash_tests, './$SOURCE')

classes = ['Board.c','PlayerHuman.c','PlayerMinMax.c']
target = env.Program("main",['main.c'] + classes,LIBS=['c','hash'])
env.Command('run', target, './$SOURCE ../maps/day1.map')
# Default(target)


test_files = Glob('*Test.c')
if(len(test_files) > 0):
	tested_files = [re.sub('Test.c','.o',f) for f in [str(s) for s in test_files]]
	env.GenerateAllTests('AllTests.c',test_files)
	alltests = env.Program(['AllTests.c'] + test_files + tested_files + cutest,LIBS=['hash'])
	runtests = Command('test',alltests, './$SOURCE')
	Default(runtests)

