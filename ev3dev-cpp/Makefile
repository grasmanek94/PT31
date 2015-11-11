empty:=

#######################################################################################################
CFLAGS_RELEASE=-O3 -ggdb3
CFLAGS_DEBUG=-O0 -ggdb3 -Wall
CFLAGS_COMBINED_RELEASE=$(CFLAGS_RELEASE) $(CFLAGS)
CFLAGS_COMBINED_DEBUG=$(CFLAGS_DEBUG) $(CFLAGS)
SUFFIX_RELEASE=_REL
SUFFIX_DEBUG=_DBG

SUFFIX_SELECTED=
CFLAGS_SELECTED=

TEMPORARY=
#######################################################################################################
CFLAGS_STATIC_LIBRARY=		-c
CFLAGS_INCLUDE_DIRS=		-I.
CFLAGS_COMMON=				-std=gnu++11 $(CFLAGS_INCLUDE_DIRS) $(CFLAGS_SELECTED)

#######################################################################################################
DEFAULT_CLEARER=
CHOSEN_GPP=
CHOSEN_NAM=				
MAIN_BASE_DIR=
GPP_EV3=					arm-none-linux-gnueabi-g++
GPP_EV3_NAM=_ev3
GPP_RSP=					arm-linux-gnueabihf-c++
GPP_RSP_NAM=_rsp
ARCHIVE=					ar rvs

#######################################################################################################
COMMON_INCLUDES=			
COMMON_DEFINITIONS=
MAIN_BASE_DIR=				./build/

#######################################################################################################
GOOGLE_TEST_INCLUDE_DIRS=	-I./libraries/googletest -I./libraries/googletest/include
GOOGLE_TEST_SOURCES=		./libraries/__linux__/google_test.cc
GOOGLE_TEST_LIBRARY=		$(MAIN_BASE_DIR)obj/google_test$(SUFFIX_SELECTED).o

GOOGLE_MOCK_INCLUDE_DIRS=	-I./libraries/googlemock -I./libraries/googlemock/include $(GOOGLE_TEST_INCLUDE_DIRS)
GOOGLE_MOCK_SOURCES=		./libraries/__linux__/google_mock.cc
GOOGLE_MOCK_LIBRARY=		$(MAIN_BASE_DIR)obj/google_mock$(SUFFIX_SELECTED).o

GLM_INCLUDE_DIRS=			-I./libraries/glm
GLM_SOURCES=				./libraries/__linux__/glm.cpp
GLM_LIBRARY=				$(MAIN_BASE_DIR)obj/glm$(SUFFIX_SELECTED).o

GoogleTest: $(GOOGLE_TEST_SOURCES)
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(GOOGLE_TEST_INCLUDE_DIRS) $(GOOGLE_TEST_SOURCES) -o $(GOOGLE_TEST_LIBRARY)
	
GoogleMock: $(GOOGLE_MOCK_SOURCES)
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(GOOGLE_MOCK_INCLUDE_DIRS) $(GOOGLE_MOCK_SOURCES) -o $(GOOGLE_MOCK_LIBRARY)

GLM: $(GLM_SOURCES)
	@mkdir -p $(MAIN_BASE_DIR)obj
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(GLM_INCLUDE_DIRS) $(GLM_SOURCES) -o $(GLM_LIBRARY)

#######################################################################################################
INCLUDE_DIRS_PROJECT=		$(GLM_INCLUDE_DIRS) \
							-I./project \
							-I./project/lib \
							-I./common
							
#######################################################################################################
LIBCOMMON_INCLUDE_DIRS=		$(INCLUDE_DIRS_PROJECT)
LIBCOMMON_SOURCES=			$(shell find ./common/ -name '*.c*')
LIBCOMMON_OBJ_DIR=			$(MAIN_BASE_DIR)libcommon/
LIBCOMMON_OBJECTS=			$(addprefix $(LIBCOMMON_OBJ_DIR),$(LIBCOMMON_SOURCES))
LIBCOMMON_LIBRARY=			$(MAIN_BASE_DIR)libcommon$(SUFFIX_SELECTED).a
LIBCOMMON_DEFINES=			$(COMMON_DEFINITIONS)

LibCommon: DEFAULT_CLEARER=$(LIBCOMMON_OBJ_DIR)
LibCommon: COMMON_INCLUDES=$(LIBCOMMON_INCLUDE_DIRS)
LibCommon: $(LIBCOMMON_OBJECTS)
	@$(ARCHIVE) $(LIBCOMMON_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) > /dev/null
	
#######################################################################################################
COMMON_LIBRARIES=			$(GLM_LIBRARY) \
							-lpthread \
							-ldl \
							-lrt \
							$(LIBCOMMON_LIBRARY)
	
#######################################################################################################
PROJECT_NAME=				project

LIBPROJECT_INCLUDE_DIRS=	$(INCLUDE_DIRS_PROJECT)
LIBPROJECT_SOURCES=			$(shell find ./$(PROJECT_NAME)/lib/ -name '*.c*')
LIBPROJECT_OBJ_DIR=			$(MAIN_BASE_DIR)lib$(PROJECT_NAME)/
LIBPROJECT_OBJECTS=			$(addprefix $(LIBPROJECT_OBJ_DIR),$(LIBPROJECT_SOURCES))
LIBPROJECT_LIBRARY=			$(MAIN_BASE_DIR)lib$(PROJECT_NAME)$(SUFFIX_SELECTED).a
LIBPROJECT_DEFINES=			$(COMMON_DEFINITIONS)

APP_PROJECT_LIBRARIES=		$(COMMON_LIBRARIES) \
							$(LIBPROJECT_LIBRARY) 
APP_PROJECT_INCLUDE_DIRS=	$(INCLUDE_DIRS_PROJECT)
APP_PROJECT_SOURCES=		$(shell find ./$(PROJECT_NAME)/ ! -path "./$(PROJECT_NAME)/lib/*" -name '*.c*')
APP_PROJECT_OBJ_DIR=		$(MAIN_BASE_DIR)obj/$(PROJECT_NAME)/
APP_PROJECT_OBJECTS=		$(addprefix $(APP_PROJECT_OBJ_DIR),$(APP_PROJECT_SOURCES))
APP_PROJECT_EXECUTABLE=		$(MAIN_BASE_DIR)$(PROJECT_NAME)$(SUFFIX_SELECTED).bin
APP_PROJECT_DEFINES=		$(COMMON_DEFINITIONS) \
							-DPROJECT_APP

LibProject: DEFAULT_CLEARER=$(LIBPROJECT_OBJ_DIR)
LibProject: COMMON_INCLUDES=$(LIBPROJECT_INCLUDE_DIRS)
LibProject: $(LIBPROJECT_OBJECTS)
	@$(ARCHIVE) $(LIBPROJECT_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) > /dev/null

AppProject: DEFAULT_CLEARER=$(APP_PROJECT_OBJ_DIR)
AppProject: COMMON_INCLUDES=$(APP_PROJECT_INCLUDE_DIRS)
AppProject: $(APP_PROJECT_OBJECTS)
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(APP_PROJECT_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(APP_PROJECT_LIBRARIES) -o $(APP_PROJECT_EXECUTABLE)

#######################################################################################################
INCLUDE_DIRS_UWB=			$(GLM_INCLUDE_DIRS) \
							-I./uwb_location \
							-I./uwb_location/lib \
							-I./common

UWB_NAME=					uwb_location

LIBUWB_LOC_INCLUDE_DIRS=	$(INCLUDE_DIRS_UWB)
LIBUWB_LOC_SOURCES=			$(shell find ./$(UWB_NAME)/lib/ -name '*.c*')
LIBUWB_LOC_OBJ_DIR=			$(MAIN_BASE_DIR)lib$(UWB_NAME)/
LIBUWB_LOC_OBJECTS=			$(addprefix $(LIBUWB_LOC_OBJ_DIR),$(LIBUWB_LOC_SOURCES))
LIBUWB_LOC_LIBRARY=			$(MAIN_BASE_DIR)lib$(UWB_NAME)$(SUFFIX_SELECTED).a
LIBUWB_LOC_DEFINES=			$(COMMON_DEFINITIONS)

UWB_LOC_LIBRARIES=			$(COMMON_LIBRARIES) \
							$(LIBUWB_LOC_LIBRARY) 
UWB_LOC_INCLUDE_DIRS=		$(INCLUDE_DIRS_UWB)
UWB_LOC_SOURCES=			$(shell find ./$(UWB_NAME)/ ! -path "./$(UWB_NAME)/lib/*" -name '*.c*')
UWB_LOC_OBJ_DIR=			$(MAIN_BASE_DIR)obj/$(UWB_NAME)/
UWB_LOC_OBJECTS=			$(addprefix $(UWB_LOC_OBJ_DIR),$(UWB_LOC_SOURCES))
UWB_LOC_EXECUTABLE=			$(MAIN_BASE_DIR)$(UWB_NAME)$(SUFFIX_SELECTED).bin
UWB_LOC_DEFINES=			$(COMMON_DEFINITIONS) \
							-DUWB_LOCATION

LibUwbLoc: DEFAULT_CLEARER=$(LIBUWB_LOC_OBJ_DIR)
LibUwbLoc: COMMON_INCLUDES=$(LIBUWB_LOC_INCLUDE_DIRS)
LibUwbLoc: $(LIBUWB_LOC_OBJECTS)
	@$(ARCHIVE) $(LIBUWB_LOC_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) > /dev/null

UwbLocation: DEFAULT_CLEARER=$(UWB_LOC_OBJ_DIR)
UwbLocation: COMMON_INCLUDES=$(UWB_LOC_INCLUDE_DIRS)
UwbLocation: $(UWB_LOC_OBJECTS)
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(UWB_LOC_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(UWB_LOC_LIBRARIES) -o $(UWB_LOC_EXECUTABLE)

#######################################################################################################
INCLUDE_DIRS_PIX=			$(GLM_INCLUDE_DIRS) \
							-I./pix_location \
							-I./pix_location/lib \
							-I./common

PIX_NAME=					pix_location

LIBPIX_LOC_INCLUDE_DIRS=	$(INCLUDE_DIRS_PIX)
LIBPIX_LOC_SOURCES=			$(shell find ./$(PIX_NAME)/lib/ -name '*.c*')
LIBPIX_LOC_OBJ_DIR=			$(MAIN_BASE_DIR)lib$(PIX_NAME)/
LIBPIX_LOC_OBJECTS=			$(addprefix $(LIBPIX_LOC_OBJ_DIR),$(LIBPIX_LOC_SOURCES))
LIBPIX_LOC_LIBRARY=			$(MAIN_BASE_DIR)lib$(PIX_NAME)$(SUFFIX_SELECTED).a
LIBPIX_LOC_DEFINES=			$(COMMON_DEFINITIONS)

PIX_LOC_LIBRARIES=			$(COMMON_LIBRARIES) \
							$(LIBPIX_LOC_LIBRARY) 
PIX_LOC_INCLUDE_DIRS=		$(INCLUDE_DIRS_PIX)
PIX_LOC_SOURCES=			$(shell find ./$(PIX_NAME)/ ! -path "./$(PIX_NAME)/lib/*" -name '*.c*')
PIX_LOC_OBJ_DIR=			$(MAIN_BASE_DIR)obj/$(PIX_NAME)/
PIX_LOC_OBJECTS=			$(addprefix $(PIX_LOC_OBJ_DIR),$(PIX_LOC_SOURCES))
PIX_LOC_EXECUTABLE=			$(MAIN_BASE_DIR)$(PIX_NAME)$(SUFFIX_SELECTED).bin
PIX_LOC_DEFINES=			$(COMMON_DEFINITIONS) \
							-DPIX_LOCATION

LibPixLoc: DEFAULT_CLEARER=$(LIBPIX_LOC_OBJ_DIR)
LibPixLoc: COMMON_INCLUDES=$(LIBPIX_LOC_INCLUDE_DIRS)
LibPixLoc: $(LIBPIX_LOC_OBJECTS)
	@$(ARCHIVE) $(LIBPIX_LOC_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) > /dev/null

PixLocation: DEFAULT_CLEARER=$(PIX_LOC_OBJ_DIR)
PixLocation: COMMON_INCLUDES=$(PIX_LOC_INCLUDE_DIRS)
PixLocation: $(PIX_LOC_OBJECTS)
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(PIX_LOC_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(PIX_LOC_LIBRARIES) -o $(PIX_LOC_EXECUTABLE)

#######################################################################################################
INCLUDE_DIRS_SVR=			$(GLM_INCLUDE_DIRS) \
							-I./server \
							-I./server/lib \
							-I./common

SVR_NAME=					server

LIBSVR_INCLUDE_DIRS=		$(INCLUDE_DIRS_SVR)
LIBSVRC_SOURCES=			$(shell find ./$(SVR_NAME)/lib/ -name '*.c*')
LIBSVR_OBJ_DIR=				$(MAIN_BASE_DIR)lib$(SVR_NAME)/
LIBSVR_OBJECTS=				$(addprefix $(LIBSVR_OBJ_DIR),$(LIBSVR_SOURCES))
LIBSVR_LIBRARY=				$(MAIN_BASE_DIR)lib$(SVR_NAME)$(SUFFIX_SELECTED).a
LIBSVR_DEFINES=				$(COMMON_DEFINITIONS)

SVR_LIBRARIES=				$(COMMON_LIBRARIES) \
							$(LIBSVR_LIBRARY) 
SVR_INCLUDE_DIRS=			$(INCLUDE_DIRS_SVR)
SVR_SOURCES=				$(shell find ./$(SVR_NAME)/ ! -path "./$(SVR_NAME)/lib/*" -name '*.c*')
SVR_OBJ_DIR=				$(MAIN_BASE_DIR)obj/$(SVR_NAME)/
SVR_OBJECTS=				$(addprefix $(SVR_OBJ_DIR),$(SVR_SOURCES))
SVR_EXECUTABLE=				$(MAIN_BASE_DIR)$(SVR_NAME)$(SUFFIX_SELECTED).bin
SVR_DEFINES=				$(COMMON_DEFINITIONS) \
							-DSVR

LibSvr: DEFAULT_CLEARER=$(LIBSVR_OBJ_DIR)
LibSvr: COMMON_INCLUDES=$(LIBSVR_INCLUDE_DIRS)
LibSvr: $(LIBSVR_OBJECTS)
	@$(ARCHIVE) $(LIBSVR_LIBRARY) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) > /dev/null

Svr: DEFAULT_CLEARER=$(SVR_OBJ_DIR)
Svr: COMMON_INCLUDES=$(SVR_INCLUDE_DIRS)
Svr: $(SVR_OBJECTS)
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(SVR_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(SVR_LIBRARIES) -o $(SVR_EXECUTABLE)

#######################################################################################################
TEST_INCLUDE_DIRS=			$(INCLUDE_DIRS_PROJECT) \
							$(INCLUDE_DIRS_UWB) \
							$(INCLUDE_DIRS_PIX) \
							$(GOOGLE_MOCK_INCLUDE_DIRS)
TEST_SOURCES=				$(shell find ./test/ -name '*.c*')
TEST_OBJ_DIR=				$(MAIN_BASE_DIR)obj/test/
TEST_OBJECTS=				$(addprefix $(TEST_OBJ_DIR), $(TEST_SOURCES))
TEST_EXECUTABLE=			$(MAIN_BASE_DIR)test$(SUFFIX_SELECTED).bin
TEST_DEFINES=				$(COMMON_DEFINITIONS) \
							-DUNIT_TESTING_FRAMEWORK
TEST_LIBRARIES=				$(COMMON_LIBRARIES) \
							$(LIBPROJECT_LIBRARY) \
							$(LIBUWB_LOC_LIBRARY) \
							$(GOOGLE_TEST_LIBRARY) \
							$(GOOGLE_MOCK_LIBRARY)

TestProject: DEFAULT_CLEARER=$(TEST_OBJ_DIR)
TestProject: COMMON_INCLUDES=$(TEST_INCLUDE_DIRS)
TestProject: $(TEST_OBJECTS)
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_INCLUDE_DIRS) $(TEST_INCLUDE_DIRS) $(addsuffix $(SUFFIX_SELECTED).o,$(addprefix ./,$^)) $(TEST_LIBRARIES) -o $(TEST_EXECUTABLE)

#######################################################################################################
all: ev3_r ev3_tr rsp_r rsp_tr

.DEFAULT:
	@mkdir -p ./$(@D)
	@$(CHOSEN_GPP) $(CFLAGS_COMMON) $(CFLAGS_STATIC_LIBRARY) $(CFLAGS_INCLUDE_DIRS) $(COMMON_INCLUDES) ./$(subst $(DEFAULT_CLEARER),$(empty),./$@) -o ./$@$(SUFFIX_SELECTED).o

clean:
	@rm -rf $(MAIN_BASE_DIR)*

ev3_d ev3_td ev3_r ev3_tr: CHOSEN_GPP=$(GPP_EV3)
ev3_d ev3_td ev3_r ev3_tr: CHOSEN_NAM=$(GPP_EV3_NAM)
	
rsp_d rsp_td rsp_r rsp_tr: CHOSEN_GPP=$(GPP_RSP)
rsp_d rsp_td rsp_r rsp_tr: CHOSEN_NAM=$(GPP_RSP_NAM)

ev3_d ev3_td rsp_d rsp_td: SUFFIX_SELECTED=$(SUFFIX_DEBUG)$(CHOSEN_NAM)
ev3_d ev3_td rsp_d rsp_td: CFLAGS_SELECTED=$(CFLAGS_COMBINED_DEBUG)

ev3_r ev3_tr rsp_r rsp_tr: SUFFIX_SELECTED=$(SUFFIX_RELEASE)$(CHOSEN_NAM)
ev3_r ev3_tr rsp_r rsp_tr: CFLAGS_SELECTED=$(CFLAGS_COMBINED_RELEASE)

ev3_d ev3_r rsp_r rsp_d: GLM LibCommon LibProject AppProject LibUwbLoc UwbLocation LibPixLoc PixLocation LibSvr Svr

ev3_td ev3_tr rsp_tr rsp_td: GoogleTest GoogleMock GLM LibCommon LibProject LibUwbLoc LibPixLoc LibSvr TestProject
#	valgrind $(TEST_EXECUTABLE)
