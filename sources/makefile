APP_NAME := SuperCartRacing11

VPATH := ./server:./client:./common
OBJDIR := ./Object
DEPDIR := ./Dependency
DEBUGDIR := ./Debug
RELEASEDIR := ./Release
OUTPUTDIR := ./

CC	:= gcc
CFLAGS  += -std=c99 `../library/bin/sdl2-config --prefix=../library --cflags`
LIBS    := `../library/bin/sdl2-config --prefix=../library --static-libs` -lsdl2_image -lsdl2_ttf -lfreetype -L/usr/X11R6/lib/


COMMON_SRCS := netlib.c imglib.c maplib.c textlib.c errorlib.c definitions.c
SV_SRCS := $(COMMON_SRCS) $(subst server/,,$(wildcard server/*.c))  
CL_SRCS := $(COMMON_SRCS) $(subst client/,,$(wildcard client/*.c))  

SV_OBJS := $(addprefix $(OBJDIR)/,$(SV_SRCS:%.c=%.o))
SV_DEPS := $(addprefix $(DEPDIR)/,$(SV_SRCS:%.c=%.d))
CL_OBJS := $(addprefix $(OBJDIR)/,$(CL_SRCS:%.c=%.o))
CL_DEPS := $(addprefix $(DEPDIR)/,$(CL_SRCS:%.c=%.d))


.PHONY:all
all: release

.PHONY:develop
develop: server.out client.out

.PHONY:server
server:
	@make server.out
.PHONY:client
client:
	@make client.out
.PHONY:debug
debug:
	@make develop CFLAGS="-g $(CFLAGS)"  OBJDIR=$(DEBUGDIR)/Object OUTPUTDIR=$(DEBUGDIR)
.PHONY:release
release:
	@make develop CFLAGS+="-O2 $(CFLAGS)" OBJDIR=$(RELEASEDIR)/Object OUTPUTDIR=$(RELEASEDIR)
	@echo "\033[0;33mGenerating App Bundle $@\033[0;39m"
	@bundle/create_app.sh ${APP_NAME}


server.out: ${SV_OBJS}
	@echo "\033[0;35mGenerating $@\033[0;39m"
	@${CC} ${CFLAGS} ${LIBS} -o $(OUTPUTDIR)/$@ ${SV_OBJS}

client.out: ${CL_OBJS}
	@echo "\033[0;35mGenerating $@\033[0;39m"
	@${CC} ${CFLAGS} ${LIBS} -o $(OUTPUTDIR)/$@ ${CL_OBJS}

$(OBJDIR)/%.o:%.c
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	@[ -d $(DEPDIR) ] || mkdir -p $(DEPDIR)
	@echo "\033[0;34mCompiling $<\033[0;39m"
	@$(CC) -MMD -MP -MF $(DEPDIR)/$*.d $(CFLAGS) -o $@ -c $<

-include $(SV_DEPS) $(CL_DEPS)


.PHONY:clean
clean:
	rm -rf $(OBJDIR) $(DEPDIR) $(DEBUGDIR) $(RELEASEDIR) $(APP_NAME).app $(APP_NAME)_Server.app
	rm server.out client.out
.PHONY:
